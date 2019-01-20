#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int SearchFirstOfK(const vector<int>& A, int k) {
	int btm = 0;
	int top = A.size() - 1;
	while (btm < top) {
		if (A[btm] == k) return btm;
		int c = btm + (top - btm) / 2;
		if (A[c] >= k) top = c;
		else btm = c + 1;
	}
	if (btm < A.size() && A[btm] == k) return btm;
	return -1;
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "A", "k" };
	return GenericTestMain(args, "search_first_key.cc", "search_first_key.tsv",
		&SearchFirstOfK, DefaultComparator{}, param_names);
}
