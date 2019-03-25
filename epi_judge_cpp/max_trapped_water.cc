#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int GetMaxTrappedWater(const vector<int>& heights) {
	int mx_h = 0;
	for (auto &h : heights) {
		if (mx_h < h) mx_h = h;
	}
	int mx_t = 0;
	for (int h = 1; h <= mx_h; h++) {
		int l = 0, r = heights.size() - 1;
		while (heights[l] < h) l++;
		while (heights[r] < h) r--;
		const int t = (r - l) * h;
		if (mx_t < t) mx_t = t;
	}
	return mx_t;
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "heights" };
	return GenericTestMain(args, "max_trapped_water.cc", "max_trapped_water.tsv",
		&GetMaxTrappedWater, DefaultComparator{}, param_names);
}
