#include <algorithm>
#include "binary_tree_node.h"
#include "test_framework/generic_test.h"

struct BalanceDetails {
	int height;
	bool balanced;
};

BalanceDetails check_balance(const unique_ptr<BinaryTreeNode<int>>& tree) {
	if (tree) {
		BalanceDetails l = check_balance(tree->left);
		if (!l.balanced) return { l.height + 1, false };
		BalanceDetails r = check_balance(tree->right);
		int h = std::max(l.height, r.height) + 1;
		return { h, r.balanced && std::abs(r.height - l.height) <= 1 };
	}
	else return { 0, true };
}


bool IsBalanced(const unique_ptr<BinaryTreeNode<int>>& tree) {
	BalanceDetails balance = check_balance(tree);
	return balance.balanced;
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "tree" };
	return GenericTestMain(args, "is_tree_balanced.cc", "is_tree_balanced.tsv",
		&IsBalanced, DefaultComparator{}, param_names);
}
