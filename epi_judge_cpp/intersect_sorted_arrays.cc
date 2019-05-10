#include <vector>
#include "test_framework/generic_test.h"

using std::vector;

vector<int> IntersectTwoSortedArrays(const vector<int> &A,
                                     const vector<int> &B) {
    auto &ai = A.cbegin();
    auto &bi = B.cbegin();
    const auto &ae = A.cend();
    const auto &be = B.cend();

    vector<int> result;
    while (ai != ae && bi != be) {
        int a = *ai;
        int b = *bi;
        if (a == b) {
            result.push_back(a);
            while (ai != ae && *ai == a) ai++;
            while (bi != be && *bi == b) bi++;
        } else (a < b ? ai : bi)++;
    }
    return result;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"A", "B"};
    return GenericTestMain(
            args, "intersect_sorted_arrays.cc", "intersect_sorted_arrays.tsv",
            &IntersectTwoSortedArrays, DefaultComparator{}, param_names);
}
