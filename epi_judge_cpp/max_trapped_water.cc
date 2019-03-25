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
		if (heights[l] < heights[r]) {
			int h = heights[l];
			while (heights[l] <= h) l++;
		}
		else if (heights[l] > heights[r]) {
			int h = heights[r];
			while (heights[r] <= h) r--;
		}
		else {
			int h = heights[r];
			while (heights[l] <= h && l < r) l++;
			while (heights[r] <= h && r > l) r--;
		}
	}
	return mx_t;
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "heights" };
	return GenericTestMain(args, "max_trapped_water.cc", "max_trapped_water.tsv",
		&GetMaxTrappedWater, DefaultComparator{}, param_names);
}
