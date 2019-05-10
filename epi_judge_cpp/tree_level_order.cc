#include <memory>
#include <vector>
#include <deque>
#include "binary_tree_node.h"
#include "test_framework/generic_test.h"

using std::unique_ptr;
using std::vector;
using std::deque;

struct tree_ptr {
    int level;
    BinaryTreeNode<int> *node;
};

vector<vector<int>> BinaryTreeDepthOrder(
        const unique_ptr<BinaryTreeNode<int>> &tree) {
    if (!tree) return {};

    deque<tree_ptr> work;
    int level = 0;
    vector<vector<int>> res;
    vector<int> layer;
    work.push_back({0, tree.get()});
    while (!work.empty()) {
        auto ptr = work.front();
        work.pop_front();
        if (level != ptr.level) {
            res.push_back(layer);
            layer.clear();
            level = ptr.level;
        }
        layer.push_back(ptr.node->data);
        if (ptr.node->left) {
            work.push_back({ptr.level + 1, ptr.node->left.get()});
        }
        if (ptr.node->right) {
            work.push_back({ptr.level + 1, ptr.node->right.get()});
        }
    }
    if (!layer.empty()) res.push_back(layer);
    return res;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"tree"};
    return GenericTestMain(args, "tree_level_order.cc", "tree_level_order.tsv",
                           &BinaryTreeDepthOrder, DefaultComparator{},
                           param_names);
}
