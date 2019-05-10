#include <vector>
#include "test_framework/generic_test.h"

using std::vector;

int SearchSmallest(const vector<int> &A) {
    if (A.size() == 1 || A[0] < A[A.size() - 1]) return 0;
    int a = 0, b = A.size() - 1;
    while (a < b - 1) {
        int mid = (a + b) / 2;
        if (A[b] > A[mid]) b = mid; else a = mid;
    }
    return b;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"A"};
    return GenericTestMain(args, "search_shifted_sorted_array.cc",
                           "search_shifted_sorted_array.tsv", &SearchSmallest,
                           DefaultComparator{}, param_names);
}
