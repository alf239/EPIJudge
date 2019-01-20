#include <vector>
#include <limits>
#include "test_framework/generic_test.h"

using std::vector;
double BuyAndSellStockOnce(const vector<double>& prices) {
	double min_so_far = std::numeric_limits<double>::infinity();
	double max_profit = 0.0;
	for (auto p : prices) {
		if (p - min_so_far > max_profit) max_profit = p - min_so_far;
		if (p < min_so_far) min_so_far = p;
	}
	return max_profit;
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "prices" };
	return GenericTestMain(args, "buy_and_sell_stock.cc",
		"buy_and_sell_stock.tsv", &BuyAndSellStockOnce,
		DefaultComparator{}, param_names);
}
