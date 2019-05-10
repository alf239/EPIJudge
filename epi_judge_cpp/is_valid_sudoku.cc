#include <vector>
#include "test_framework/generic_test.h"

using std::vector;

// Check if a partially filled matrix has any conflicts.
bool IsValidSudoku(const vector<vector<int>> &partial_assignment) {
    int seen;
    for (int i = 0; i < 9; i++) {
        seen = 0;
        for (int j = 0; j < 9; j++) {
            int x = partial_assignment[i][j];
            if (x == 0) continue;
            int mask = 1 << x;
            if (mask & seen) return false;
            seen |= mask;
        }
        seen = 0;
        for (int j = 0; j < 9; j++) {
            int x = partial_assignment[j][i];
            if (x == 0) continue;
            int mask = 1 << x;
            if (mask & seen) return false;
            seen |= mask;
        }
        seen = 0;
        for (int j = 0; j < 9; j++) {
            int x = partial_assignment[(i % 3) * 3 + j % 3][(i / 3) * 3 + j / 3];
            if (x == 0) continue;
            int mask = 1 << x;
            if (mask & seen) return false;
            seen |= mask;
        }
    }
    return true;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"partial_assignment"};
    return GenericTestMain(args, "is_valid_sudoku.cc", "is_valid_sudoku.tsv",
                           &IsValidSudoku, DefaultComparator{}, param_names);
}
