#include <memory>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/timed_executor.h"

using std::unique_ptr;
using std::queue;

template<typename T>
struct BinaryTreeNode {
    T data;
    unique_ptr<BinaryTreeNode<T>> left, right;
    BinaryTreeNode<T> *next = nullptr;  // Populates this field.

    explicit BinaryTreeNode(T data) : data(data) {};
};

template<typename T>
struct QEntry {
    BinaryTreeNode<int> *tree;
    int level;
};


void ConstructRightSibling(BinaryTreeNode<int> *tree) {
    if (tree == nullptr) return;

    std::queue<QEntry<int>> q;
    QEntry<int> prev{nullptr, -1};
    q.push({tree, 0});
    while (!q.empty()) {
        QEntry<int> e = q.front();
        q.pop();

        if (e.tree->left != nullptr) {
            q.push({e.tree->left.get(), e.level + 1});
        }
        if (e.tree->right != nullptr) {
            q.push({e.tree->right.get(), e.level + 1});
        }
        if (prev.level == e.level) {
            prev.tree->next = e.tree;
        }

        prev = e;
    }
}

template<>
struct SerializationTraits<unique_ptr<BinaryTreeNode<int>>>
        : BinaryTreeSerializationTraits<unique_ptr<BinaryTreeNode<int>>, false> {
};

std::vector<std::vector<int>> ConstructRightSiblingWrapper(
        TimedExecutor &executor, unique_ptr<BinaryTreeNode<int>> &tree) {
    executor.Run([&] { ConstructRightSibling(tree.get()); });

    std::vector<std::vector<int>> result;
    auto level_start = tree.get();
    while (level_start) {
        result.emplace_back();
        auto level_iter = level_start;
        while (level_iter) {
            result.back().push_back(level_iter->data);
            level_iter = level_iter->next;
        }
        level_start = level_start->left.get();
    }
    return result;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"executor", "tree"};
    return GenericTestMain(
            args, "tree_right_sibling.cc", "tree_right_sibling.tsv",
            &ConstructRightSiblingWrapper, DefaultComparator{}, param_names);
}
