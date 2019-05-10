#include <string>
#include <vector>
#include <algorithm>
#include "test_framework/generic_test.h"

using std::string;
using std::vector;
using std::min;

int LevenshteinDistance(const string &A, const string &B) {
    vector<int> dp(A.size() + 1);
    for (int j = 0; j <= A.size(); j++) {
        dp[j] = j;
    }
    for (int i = 1; i <= B.size(); i++) {
        int diag = dp[0]++;
        for (int j = 1; j <= A.size(); j++) {
            int replace = diag + (A[j - 1] == B[i - 1] ? 0 : 1);
            diag = dp[j];
            int remove = dp[j] + 1;
            int insert = dp[j - 1] + 1;
            dp[j] = min(replace, min(remove, insert));
        }
    }
    return dp[A.size()];
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"A", "B"};
    return GenericTestMain(args, "levenshtein_distance.cc",
                           "levenshtein_distance.tsv", &LevenshteinDistance,
                           DefaultComparator{}, param_names);
}
