#include <vector>
#include "test_framework/generic_test.h"

using std::vector;

void generate(vector<vector<int>> *acc, const vector<int> &A, vector<int> *attempt, vector<bool> *used, int n) {
    if (n == A.size()) {
        acc->push_back(*attempt);
        return;
    }

    for (int i = 0; i < A.size(); i++) {
        if (!(*used)[i]) {
            (*used)[i] = true;
            (*attempt)[n] = A[i];
            generate(acc, A, attempt, used, n + 1);
            (*used)[i] = false;
        }
    }
}

vector<vector<int>> Permutations(vector<int> A) {
    vector<vector<int>> result;
    vector<int> attempt(A.size());
    vector<bool> used(A.size(), false);

    generate(&result, A, &attempt, &used, 0);

    return result;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"A"};
    return GenericTestMain(
            args, "permutations.cc", "permutations.tsv", &Permutations,
            &UnorderedComparator<vector<vector<int>>>, param_names);
}
