#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

// Given n, return all primes up to and including n.
vector<int> GeneratePrimes(int n) {
	vector<bool> sieve(n + 1, true);
	for (int i = 2; i <= std::sqrt(n + 1); i++) {
		if (!sieve[i]) continue;
		for (int j = 2; j <= n / i; j++) {
			sieve[j*i] = false;
		}
	}
	vector<int> result;
	for (int i = 2; i <= n; i++) {
		if (sieve[i]) result.push_back(i);
	}
	return result;
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "n" };
	return GenericTestMain(args, "prime_sieve.cc", "prime_sieve.tsv",
		&GeneratePrimes, DefaultComparator{}, param_names);
}
