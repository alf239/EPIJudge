#include <vector>
#include "test_framework/generic_test.h"

using std::vector;

void MergeTwoSortedArrays(vector<int> &A, int m, const vector<int> &B, int n) {
    for (int i = m + n; n > 0 && --i >= 0;) {
        if (m > 0 && A[m - 1] > B[n - 1]) A[i] = A[m-- - 1];
        else A[i] = B[n-- - 1];
    }
}

vector<int> MergeTwoSortedArraysWrapper(vector<int> A, int m,
                                        const vector<int> &B, int n) {
    MergeTwoSortedArrays(A, m, B, n);
    return A;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"A", "m", "B", "n"};
    return GenericTestMain(
            args, "two_sorted_arrays_merge.cc", "two_sorted_arrays_merge.tsv",
            &MergeTwoSortedArraysWrapper, DefaultComparator{}, param_names);
}
