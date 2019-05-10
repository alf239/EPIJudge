#include <memory>
#include <vector>
#include "bst_node.h"
#include "test_framework/generic_test.h"

using std::unique_ptr;
using std::vector;

void CollectKLargestInBST(vector<int> *acc, BstNode<int> *tree, int *k) {
    if (!tree) return;
    CollectKLargestInBST(acc, tree->right.get(), k);
    if (*k == 0) return;
    acc->push_back(tree->data);
    (*k)--;
    if (*k == 0) return;
    CollectKLargestInBST(acc, tree->left.get(), k);
}

vector<int> FindKLargestInBST(const unique_ptr<BstNode<int>> &tree, int k) {
    vector<int> result;
    CollectKLargestInBST(&result, tree.get(), &k);
    return result;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"tree", "k"};
    return GenericTestMain(args, "k_largest_values_in_bst.cc",
                           "k_largest_values_in_bst.tsv", &FindKLargestInBST,
                           &UnorderedComparator<std::vector<int>>, param_names);
}
