#include <vector>
#include "test_framework/generic_test.h"

using std::vector;

int NumberOfWays(int n, int m) {
	vector<int> dp(n, 1);
	for (int i = 1; i < m; i++) {
		for (int j = 1; j < n; j++) {
			dp[j] += dp[j - 1];
		}
	}
	return dp[n - 1];
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "n", "m" };
	return GenericTestMain(args, "number_of_traversals_matrix.cc",
		"number_of_traversals_matrix.tsv", &NumberOfWays,
		DefaultComparator{}, param_names);
}
