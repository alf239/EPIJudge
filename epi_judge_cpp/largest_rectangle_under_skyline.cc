#include <vector>
#include "test_framework/generic_test.h"

using std::vector;

int CalculateLargestRectangle(const vector < int > &heights) {
	int hprev = 0, size = 0;
	for (int i = 0; i < heights.size(); i++) {
		for (int h = heights[i]; h > hprev; h--) { 
			int w = 0; 
			while (i + w < heights.size() && heights[i + w] >= h) {
				w++;
			}
			if (size < h * w) {
				size = h * w;
			}
		}
		hprev = heights[i];
	}
	return size;
}

int main(int argc, char *argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "heights" };
	return GenericTestMain(args, "largest_rectangle_under_skyline.cc",
		"largest_rectangle_under_skyline.tsv",
		&CalculateLargestRectangle, DefaultComparator{},
		param_names);
}
