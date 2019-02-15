#include <vector>
#include <functional>
#include <limits>
#include "binary_tree_with_parent_prototype.h"
#include "test_framework/generic_test.h"
using std::vector;
using std::pair;
using std::function;

// 1. left - step left
// 2. up - consume the node, try right-left
// 3. once right, and left again

struct State {
	BinaryTreeNode<int> * prev;
	BinaryTreeNode<int> * node;
};

State right_or_up(State state) {
	if (state.node->right) return { state.node, state.node->right.get() };
	else return { state.node, state.node->parent };
}

State traverse(State state, const function<void(int)> &consume) {
	if (state.prev == state.node->parent) {
		if (state.node->left) return { state.node, state.node->left.get() };
		else {
			consume(state.node->data);
			return right_or_up(state);
		}
	}
	else if (state.prev == state.node->left.get()) {
		consume(state.node->data);
		return right_or_up(state);
	}
	else {
		return { state.node, state.node->parent };
	}
}

vector<int> InorderTraversal(const unique_ptr<BinaryTreeNode<int>>& tree) {
	vector<int> result;

	State state{ nullptr, tree.get() };
	while (state.node) {
		state = traverse(state, [&result](int x) -> void { result.push_back(x); });
	}

	return result;
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "tree" };
	return GenericTestMain(args, "tree_with_parent_inorder.cc",
		"tree_with_parent_inorder.tsv", &InorderTraversal,
		DefaultComparator{}, param_names);
}
