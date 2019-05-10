#include <memory>
#include <algorithm>
#include <limits>
#include "binary_tree_node.h"
#include "test_framework/generic_test.h"

using std::unique_ptr;

struct bst_property {
    bool ok, empty;
    int min, max;
};

bst_property check_bst_property(const unique_ptr<BinaryTreeNode<int>> &tree) {
    if (tree == nullptr) return {true, true, std::numeric_limits<int>::max(), std::numeric_limits<int>::min()};

    auto l = check_bst_property(tree->left);
    auto r = check_bst_property(tree->right);

    int x = tree->data;

    return {
            l.ok && r.ok && (l.empty || x >= l.max) && (r.empty || x <= r.min),
            false,
            std::min(x, std::min(r.min, l.min)),
            std::max(x, std::max(r.max, l.max))
    };
}

bool IsBinaryTreeBST(const unique_ptr<BinaryTreeNode<int>> &tree) {
    return check_bst_property(tree).ok;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"tree"};
    return GenericTestMain(args, "is_tree_a_bst.cc", "is_tree_a_bst.tsv",
                           &IsBinaryTreeBST, DefaultComparator{}, param_names);
}
