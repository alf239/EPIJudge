#include <algorithm>
#include <iterator>
#include <vector>
#include "test_framework/generic_test.h"

using std::vector;


void layoutNQueens(const vector<vector<bool>> &board, int m, const vector<int> &queens, vector<vector<int>> *result) {
    if (m == board.size()) {
        result->push_back(queens);
        return;
    }
    for (int i = 0; i < board.size(); i++) {
        if (!board[m][i]) {
            vector<vector<bool>> brd = board;
            vector<int> qs = queens;
            qs.push_back(i);
            for (int j = m + 1; j < brd.size(); j++) {
                brd[j][i] = true;
                if (i + j - m < brd.size()) brd[j][i + j - m] = true;
                if (i - j + m >= 0) brd[j][i - j + m] = true;
            }
            layoutNQueens(brd, m + 1, qs, result);
        }
    }
}

vector<vector<int>> NQueens(int n) {
    vector<vector<int>> res;
    vector<vector<bool>> board(n, vector<bool>(n, false));
    vector<int> queens;
    layoutNQueens(board, 0, queens, &res);
    return res;
}

bool Comp(vector<vector<int>> &a, vector<vector<int>> &b) {
    std::sort(std::begin(a), std::end(a));
    std::sort(std::begin(b), std::end(b));
    return a == b;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"n"};
    return GenericTestMain(args, "n_queens.cc", "n_queens.tsv", &NQueens, &Comp,
                           param_names);
}
