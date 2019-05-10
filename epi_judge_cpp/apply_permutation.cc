#include <vector>
#include "test_framework/generic_test.h"

using std::vector;

bool is_leftmost(const vector<int> &perm, int i) {
    int j = i;
    do {
        j = perm[j];
        if (j < i) return false;
    } while (j != i);
    return true;
}

void ApplyPermutation(vector<int> *perm_ptr, vector<int> *A_ptr) {
    auto &A = *A_ptr;
    auto &perm = *perm_ptr;
    int i = 1;
    for (int i = 0; i < A.size(); i++) {
        if (is_leftmost(perm, i)) {
            int j = i;
            int saved = A[i];
            do {
                int jj = j;
                j = perm[j];
                int t = A[j];
                A[j] = saved;
                saved = t;
            } while (j != i);
        }
    }
}

vector<int> ApplyPermutationWrapper(vector<int> perm, vector<int> A) {
    ApplyPermutation(&perm, &A);
    return A;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"perm", "A"};
    return GenericTestMain(args, "apply_permutation.cc", "apply_permutation.tsv",
                           &ApplyPermutationWrapper, DefaultComparator{},
                           param_names);
}
