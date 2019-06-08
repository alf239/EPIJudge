#include <vector>
#include "test_framework/generic_test.h"

using std::vector;

bool search(const vector<vector<int>> &A, int x, int top, int bottom, int left, int right) {
    if (top > bottom) return false;
    if (left > right) return false;
    
    const int tr = A[top][right];
    
    if (x > tr) return search(A, x, top + 1, bottom, left, right);
    if (x < tr) return search(A, x, top, bottom, left, right - 1);
    return true;
}

bool MatrixSearch(const vector<vector<int>> &A, int x) {
    if (A.empty()) return false;
    if (A[0].empty()) return false;
    auto N = A.size(), M = A[0].size();

    return search(A, x, 0, N - 1, 0, M - 1);
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"A", "x"};
    return GenericTestMain(args, "search_row_col_sorted_matrix.cc",
                           "search_row_col_sorted_matrix.tsv", &MatrixSearch,
                           DefaultComparator{}, param_names);
}
