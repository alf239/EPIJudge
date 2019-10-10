#include <string>
#include <vector>
#include <unordered_set>
#include "test_framework/generic_test.h"

using std::string;
using std::vector;
using std::unordered_set;

vector<string> GenerateBalancedParentheses(int num_pairs) {
    if (num_pairs == 0) return { "" };
    vector<vector<string>> partial;
    for (int i = 0; i < num_pairs; i++) {
        partial.emplace_back(GenerateBalancedParentheses(i));
    }
    unordered_set<string> result;
    for (int i = 1; i < num_pairs; i++) {
        for (auto s1 : partial[i]) {
            for (auto s2 : partial[num_pairs - i]) {
                result.emplace(s1 + s2);
                if (i != num_pairs - i) {
                    result.emplace(s2 + s1);
                }
            }
        }
    }
    for (auto s : partial[num_pairs - 1]) {
        result.emplace("(" + s + ")");
    }
    return { result.cbegin(), result.cend() };
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"num_pairs"};
    return GenericTestMain(args, "enumerate_balanced_parentheses.cc",
                           "enumerate_balanced_parentheses.tsv",
                           &GenerateBalancedParentheses,
                           &UnorderedComparator<vector<string>>, param_names);
}
