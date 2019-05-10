#include <stdexcept>
#include <string>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"

using std::string;
using std::vector;

void fill(vector<vector<char>> *board_ptr, char empty, char paint, int x, int y) {
    auto &A = *board_ptr;
    if (x < 0 || y < 0 || x >= A.size() || y >= A[x].size() || A[x][y] != empty) return;

    A[x][y] = paint;
    fill(board_ptr, empty, paint, x - 1, y);
    fill(board_ptr, empty, paint, x + 1, y);
    fill(board_ptr, empty, paint, x, y - 1);
    fill(board_ptr, empty, paint, x, y + 1);
}

void replace(vector<vector<char>> *board_ptr, char from, char to) {
    auto &A = *board_ptr;
    for (int i = 0; i < A.size(); i++) {
        for (int j = 0; j < A[i].size(); j++) {
            if (A[i][j] == from) A[i][j] = to;
        }
    }
}

void FillSurroundedRegions(vector<vector<char>> *board_ptr) {
    auto &A = *board_ptr;
    for (int i = 0; i < A.size(); i++) {
        fill(board_ptr, 'W', '*', i, 0);
        fill(board_ptr, 'W', '*', i, A[i].size() - 1);
    }
    for (int i = 0; i < A[0].size(); i++) {
        fill(board_ptr, 'W', '*', 0, i);
    }
    for (int i = 0; i < A[A.size() - 1].size(); i++) {
        fill(board_ptr, 'W', '*', A.size() - 1, i);
    }
    replace(board_ptr, 'W', 'B');
    replace(board_ptr, '*', 'W');
}

vector<vector<string>> FillSurroundedRegionsWrapper(
        TimedExecutor &executor, vector<vector<string>> board) {
    vector<vector<char>> char_vector;
    char_vector.resize(board.size());
    for (int i = 0; i < board.size(); i++) {
        for (const string &s : board[i]) {
            if (s.size() != 1) {
                throw std::runtime_error("String size is not 1");
            }
            char_vector[i].push_back(s[0]);
        }
    }

    executor.Run([&] { FillSurroundedRegions(&char_vector); });

    board.clear();
    board.resize(char_vector.size(), {});
    for (int i = 0; i < board.size(); i++) {
        for (char c : char_vector[i]) {
            board[i].emplace_back(1, c);
        }
    }

    return board;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"executor", "board"};
    return GenericTestMain(
            args, "matrix_enclosed_regions.cc", "matrix_enclosed_regions.tsv",
            &FillSurroundedRegionsWrapper, DefaultComparator{}, param_names);
}
