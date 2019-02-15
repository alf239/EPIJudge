#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
using std::vector;

struct DuplicateAndMissing {
	int duplicate, missing;
};


DuplicateAndMissing FindDuplicateMissing(const vector<int>& A) {
	long long n = A.size();
	long long true_sum = n * (n - 1) / 2;
	long long true_sum_sq = (n - 1) * (2 * n - 1) * n / 6;
	long long sum = 0, sq_sum = 0;
	for (const auto &a : A) { sum += a;  sq_sum += a * a; }
	long long dbl_minus_missing = sum - true_sum;
	long long dbl_sq_minus_missing_sq = sq_sum - true_sum_sq;
	long long dbl_plus_missing = dbl_sq_minus_missing_sq / dbl_minus_missing;

	return {
		(int)((dbl_plus_missing + dbl_minus_missing) / 2),
		(int)((dbl_plus_missing - dbl_minus_missing) / 2) };
}
template <>
struct SerializationTraits<DuplicateAndMissing>
	: UserSerTraits<DuplicateAndMissing, int, int> {};

bool operator==(const DuplicateAndMissing& lhs,
	const DuplicateAndMissing& rhs) {
	return std::tie(lhs.duplicate, lhs.missing) ==
		std::tie(rhs.duplicate, rhs.missing);
}

std::ostream& operator<<(std::ostream& out, const DuplicateAndMissing& x) {
	return out << "duplicate: " << x.duplicate << ", missing: " << x.missing;
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "A" };
	return GenericTestMain(
		args, "search_for_missing_element.cc", "find_missing_and_duplicate.tsv",
		&FindDuplicateMissing, DefaultComparator{}, param_names);
}
