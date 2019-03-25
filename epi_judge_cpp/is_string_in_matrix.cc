#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

bool can_find_at(const vector<vector<int>>& grid, const vector<int>& pattern, int i, int j, int offset) {
	if (offset >= pattern.size()) return true;
	if (i < 0 || j < 0 || i >= grid.size() || j >= grid[i].size()) return false;
	if (grid[i][j] != pattern[offset]) return false;
	return can_find_at(grid, pattern, i - 1, j, offset + 1) ||
		can_find_at(grid, pattern, i, j - 1, offset + 1) ||
		can_find_at(grid, pattern, i + 1, j, offset + 1) ||
		can_find_at(grid, pattern, i, j + 1, offset + 1);
}

bool IsPatternContainedInGrid(const vector<vector<int>>& grid, const vector<int>& pattern) {
	for (int i = 0; i < grid.size(); i++) {
		for (int j = 0; j < grid[i].size(); j++) {
			if (can_find_at(grid, pattern, i, j, 0)) return true;
		}
	}
	return false;
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "grid", "pattern" };
	return GenericTestMain(args, "is_string_in_matrix.cc",
		"is_string_in_matrix.tsv", &IsPatternContainedInGrid,
		DefaultComparator{}, param_names);
}
