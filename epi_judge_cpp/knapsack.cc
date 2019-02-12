#include <vector>
#include <algorithm>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
using std::vector;
using std::max;

struct Item {
	int weight, value;
};

int OptimumSubjectToCapacity(const vector<Item>& items, int capacity) {
	vector<vector<int>> dp(capacity + 1, vector<int>(items.size() + 1, 0));

	for (int i = 1; i <= capacity; i++) {
		for (int j = 1; j <= items.size(); j++) {
			int w = items[j - 1].weight;
			int v = items[j - 1].value;
			int with = w > i ? 0 : (dp[i - w][j - 1] + v);
			int without = dp[i][j - 1];
			dp[i][j] = max(with, without);
		}
	}

	return dp[capacity][items.size()];
}
template <>
struct SerializationTraits<Item> : UserSerTraits<Item, int, int> {};

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "items", "capacity" };
	return GenericTestMain(args, "knapsack.cc", "knapsack.tsv",
		&OptimumSubjectToCapacity, DefaultComparator{},
		param_names);
}
