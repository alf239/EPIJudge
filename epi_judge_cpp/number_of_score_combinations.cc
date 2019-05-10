#include <vector>
#include <algorithm>
#include "test_framework/generic_test.h"

using std::vector;

int NumCombinationsForFinalScore(int final_score,
                                 const vector<int> &individual_play_scores) {
    vector<int> scores = individual_play_scores;
    vector<int> dp(final_score + 1, 0);
    std::sort(scores.begin(), scores.end());

    dp[0] = 1;
    for (int score : scores) {
        for (int i = score; i <= final_score; i++) {
            dp[i] += dp[i - score];
        }
    }

    return dp[final_score];
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"final_score", "individual_play_scores"};
    return GenericTestMain(args, "number_of_score_combinations.cc",
                           "number_of_score_combinations.tsv",
                           &NumCombinationsForFinalScore, DefaultComparator{},
                           param_names);
}
