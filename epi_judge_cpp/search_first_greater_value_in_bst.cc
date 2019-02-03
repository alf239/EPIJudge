#include <memory>
#include "bst_node.h"
#include "test_framework/generic_test.h"
using std::unique_ptr;

BstNode<int>* FindFirstGreaterThanK(BstNode<int>* current, BstNode<int>* bigger, int k) {
	if (!current) return bigger;
	if (current->data <= k) return FindFirstGreaterThanK(current->right.get(), bigger, k);
	return FindFirstGreaterThanK(current->left.get(), current, k);
}

BstNode<int>* FindFirstGreaterThanK(const unique_ptr<BstNode<int>>& tree,
	int k) {
	return FindFirstGreaterThanK(tree.get(), nullptr, k);
}

int FindFirstGreaterThanKWrapper(const unique_ptr<BstNode<int>>& tree, int k) {
	auto result = FindFirstGreaterThanK(tree, k);
	return result ? result->data : -1;
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "tree", "k" };
	return GenericTestMain(args, "search_first_greater_value_in_bst.cc",
		"search_first_greater_value_in_bst.tsv",
		&FindFirstGreaterThanKWrapper, DefaultComparator{},
		param_names);
}
