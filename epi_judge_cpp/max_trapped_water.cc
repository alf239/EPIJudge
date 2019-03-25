#include <vector>
#include <algorithm>
#include "test_framework/generic_test.h"
using std::vector;
using std::min;

int GetMaxTrappedWater(const vector<int>& heights) {
	int l = 0, r = heights.size() - 1;
	int mx_t = 0;
	while (r > l) {
		int t = (r - l) * min(heights[l], heights[r]);
		if (mx_t < t) mx_t = t;
		const int hl = heights[l];
		const int hr = heights[r];
		if (hr >= hl) while (l < r && heights[l] <= hl) l++;
		if (hl >= hr) while (l < r && heights[r] <= hr) r--;
	}
	return mx_t;
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "heights" };
	return GenericTestMain(args, "max_trapped_water.cc", "max_trapped_water.tsv",
		&GetMaxTrappedWater, DefaultComparator{}, param_names);
}
