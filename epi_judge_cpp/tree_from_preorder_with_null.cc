#include <string>
#include <vector>
#include "binary_tree_node.h"
#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"

using std::string;
using std::vector;

unique_ptr<BinaryTreeNode<int>> ReconstructPreorder(const vector<int *> &preorder) {
    if (preorder.empty() || preorder[0] == nullptr) return nullptr;

    unique_ptr<BinaryTreeNode<int>> root = std::make_unique<BinaryTreeNode<int>>(*(preorder[0]));
    vector<BinaryTreeNode<int> *> path;
    auto current = root.get();
    path.emplace_back(nullptr);
    path.emplace_back(current);
    for (auto i = preorder.cbegin() + 1; i != preorder.cend(); i++) {
        auto &elt = *i;
        if (elt != nullptr) {
            if (current == path.back()) {
                current->left = std::make_unique<BinaryTreeNode<int>>(*elt);
                current = current->left.get();
            } else {
                current->right = std::make_unique<BinaryTreeNode<int>>(*elt);
                current = current->right.get();
            }
            path.push_back(current);
        } else {
            current = path.back();
            path.pop_back();
        }
    }
    return std::move(root);
}

unique_ptr<BinaryTreeNode<int>> ReconstructPreorderWrapper(
        TimedExecutor &executor, const vector<string> &preorder) {
    vector<int> values;
    vector<int *> ptrs;
    values.reserve(preorder.size());
    for (auto &s : preorder) {
        if (s == "null") {
            ptrs.push_back(nullptr);
        } else {
            int i = std::stoi(s);
            values.push_back(i);
            ptrs.push_back(&values.back());
        }
    }

    return executor.Run([&] { return ReconstructPreorder(ptrs); });
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"executor", "preorder"};
    return GenericTestMain(args, "tree_from_preorder_with_null.cc",
                           "tree_from_preorder_with_null.tsv",
                           &ReconstructPreorderWrapper, DefaultComparator{},
                           param_names);
}
