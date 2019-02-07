#include <queue>
#include "binary_tree_node.h"
#include "test_framework/generic_test.h"

bool IsSymmetric(const unique_ptr<BinaryTreeNode<int>>& tree) {
	std::queue<std::pair<BinaryTreeNode<int>*, BinaryTreeNode<int>*>> work;

	if (!tree) return true;

	if (!tree->left || !tree->right) {
		return tree->left == tree->right;
	}

	work.push({ tree->left.get(), tree->right.get() });
	while (work.size()) {
		auto pair = work.front();
		work.pop();
		auto l = pair.first, r = pair.second;

		if (l->data != r->data) return false;

		if (l->right && r->left) work.push({ l->right.get(), r->left.get() });
		else if (l->right != r->left) return false;

		if (l->left && r->right) work.push({ l->left.get(), r->right.get() });
		else if (l->left != r->right) return false;
	}

	return true;
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "tree" };
	return GenericTestMain(args, "is_tree_symmetric.cc", "is_tree_symmetric.tsv",
		&IsSymmetric, DefaultComparator{}, param_names);
}
