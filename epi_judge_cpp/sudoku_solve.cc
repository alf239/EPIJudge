#include <algorithm>
#include <cmath>
#include <iterator>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"

using std::begin;
using std::end;
using std::vector;

bool solve(vector<vector<int>> *partial_assignment, int pos) {
    if (pos >= 81) return true;
    const int r = pos / 9;
    const int c = pos % 9;
    const int s = (r / 3) * 3 + c / 3;
    const int sr = (s / 3) * 3;
    const int sc = (s % 3) * 3;
    const int n = (r % 3) * 3 + c % 3;

    vector<vector<int>> &A = *partial_assignment;
    int &current = A[r][c];
    if (current != 0) return solve(partial_assignment, pos + 1);

    for (current = 1; current <= 9; current++) {
        bool conflicts = false;
        for (int i = 0; i < 9; i++) {
            if (i != c && A[r][i] == current) {
                conflicts = true;
                break;
            }
            if (i != r && A[i][c] == current) {
                conflicts = true;
                break;
            }
            if (i != n && A[sr + i / 3][sc + i % 3] == current) {
                conflicts = true;
                break;
            }
        }
        if (!conflicts && solve(partial_assignment, pos + 1)) return true; // keep the assignment
    }
    current = 0; // we failed this one, unassign
    return false; // and backtrack
}

bool SolveSudoku(vector<vector<int>> *partial_assignment) {
    return solve(partial_assignment, 0);
}

vector<int> GatherColumn(const vector<vector<int>> &data, size_t i) {
    vector<int> result;
    for (auto &row : data) {
        result.push_back(row[i]);
    }
    return result;
}

vector<int> GatherSquareBlock(const vector<vector<int>> &data,
                              size_t block_size, size_t n) {
    vector<int> result;
    size_t block_x = n % block_size;
    size_t block_y = n / block_size;
    for (size_t i = block_x * block_size; i < (block_x + 1) * block_size; i++) {
        for (size_t j = block_y * block_size; j < (block_y + 1) * block_size; j++) {
            result.push_back(data[i][j]);
        }
    }

    return result;
}

void AssertUniqueSeq(const vector<int> &seq) {
    vector<bool> seen(seq.size(), false);
    for (auto &x : seq) {
        if (x == 0) {
            throw TestFailure("Cell left uninitialized");
        }
        if (x < 0 || x > seq.size()) {
            throw TestFailure("Cell value out of range");
        }
        if (seen[x - 1]) {
            throw TestFailure("Duplicate value in section");
        }
        seen[x - 1] = true;
    }
}

void SolveSudokuWrapper(TimedExecutor &executor,
                        const vector<vector<int>> &partial_assignment) {
    vector<vector<int>> solved = partial_assignment;

    executor.Run([&] { SolveSudoku(&solved); });

    if (!std::equal(begin(partial_assignment), end(partial_assignment),
                    begin(solved), end(solved), [](auto br, auto cr) {
                return std::equal(begin(br), end(br), begin(cr), end(cr),
                                  [](int bcell, int ccell) {
                                      return bcell == 0 || bcell == ccell;
                                  });
            }))
        throw TestFailure("Initial cell assignment has been changed");

    auto block_size = static_cast<size_t>(sqrt(solved.size()));

    for (size_t i = 0; i < solved.size(); i++) {
        AssertUniqueSeq(solved[i]);
        AssertUniqueSeq(GatherColumn(solved, i));
        AssertUniqueSeq(GatherSquareBlock(solved, block_size, i));
    }
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"executor", "partial_assignment"};
    return GenericTestMain(args, "sudoku_solve.cc", "sudoku_solve.tsv",
                           &SolveSudokuWrapper, DefaultComparator{}, param_names);
}
