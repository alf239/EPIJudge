#include <vector>
#include "test_framework/generic_test.h"

using std::vector;

int LongestNondecreasingSubsequenceLength(const vector<int> &A) {
  vector<int> dp;
  for (int i = 0; i < A.size(); i++) {
    int max_l = 1;
    for (int j = i-1; j >= 0; j--) {
      if (A[i] >= A[j] && dp[j] >= max_l) max_l = dp[j] + 1;
    }
    dp.push_back(max_l);
  }
  int result = 0;
  for (int i = 0; i < dp.size(); i++) {
    if (dp[i] > result) result = dp[i];
  }
  return result;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"A"};
    return GenericTestMain(args, "longest_nondecreasing_subsequence.cc",
                           "longest_nondecreasing_subsequence.tsv",
                           &LongestNondecreasingSubsequenceLength,
                           DefaultComparator{}, param_names);
}
