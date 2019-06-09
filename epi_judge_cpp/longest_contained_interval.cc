#include <set>
#include "test_framework/generic_test.h"

using std::vector;

int LongestContainedRange(const vector<int> &A) {
    std::unordered_set<int> seen;
    int maxLen = 0;
    for (const auto a : A) seen.emplace(a);
    for (const auto a : A) {
        int count = 1;
        for (int aa = a - 1; seen.erase(aa) > 0; aa--) count++;
        for (int aa = a + 1; seen.erase(aa) > 0; aa++) count++;
        if (count > maxLen) maxLen = count;
    }
    return maxLen;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"A"};
    return GenericTestMain(
            args, "longest_contained_interval.cc", "longest_contained_interval.tsv",
            &LongestContainedRange, DefaultComparator{}, param_names);
}
