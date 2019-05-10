#include <vector>
#include <unordered_set>>
#include <algorithm>
#include "test_framework/generic_test.h"

using std::vector;

bool HasThreeSum(vector<int> A, int t) {
    std::unordered_set<int> pairs;
    for (int i = 0; i < A.size(); i++) {
        int a = A[i];
        for (int j = 0; j <= i; j++) {
            pairs.emplace(A[j] + a);
        }
        if (a * 3 == t || pairs.count(t - a)) {
            return true;
        }
    }
    return false;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"A", "t"};
    return GenericTestMain(args, "three_sum.cc", "three_sum.tsv", &HasThreeSum,
                           DefaultComparator{}, param_names);
}
