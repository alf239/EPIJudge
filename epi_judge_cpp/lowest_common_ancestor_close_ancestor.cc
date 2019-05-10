#include <memory>
#include <set>
#include "binary_tree_with_parent_prototype.h"
#include "test_framework/binary_tree_utils.h"
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"

using std::unique_ptr;
using std::unordered_set;

BinaryTreeNode<int> *LCA(const unique_ptr<BinaryTreeNode<int>> &node0,
                         const unique_ptr<BinaryTreeNode<int>> &node1) {
    if (!node0 || !node1) return nullptr;

    unordered_set<BinaryTreeNode<int> *> p0;
    unordered_set<BinaryTreeNode<int> *> p1;

    BinaryTreeNode<int> *n0 = node0.get();
    BinaryTreeNode<int> *n1 = node1.get();

    while (true) {
        p0.emplace(n0);
        p1.emplace(n1);
        if (p0.count(n1)) return n1;
        if (p1.count(n0)) return n0;
        if (n0) n0 = n0->parent;
        if (n1) n1 = n1->parent;
    }

    return nullptr;
}

int LcaWrapper(TimedExecutor &executor,
               const unique_ptr<BinaryTreeNode<int>> &tree, int key0,
               int key1) {
    const unique_ptr<BinaryTreeNode<int>> &node0 = MustFindNode(tree, key0);
    const unique_ptr<BinaryTreeNode<int>> &node1 = MustFindNode(tree, key1);

    auto result = executor.Run([&] { return LCA(node0, node1); });

    if (!result) {
        throw TestFailure("Result can not be nullptr");
    }
    return result->data;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"executor", "tree", "key0", "key1"};
    return GenericTestMain(args, "lowest_common_ancestor_close_ancestor.cc",
                           "lowest_common_ancestor.tsv", &LcaWrapper,
                           DefaultComparator{}, param_names);
}
