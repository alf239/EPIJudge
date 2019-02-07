#include <vector>
#include "binary_tree_node.h"
#include "test_framework/binary_tree_utils.h"
#include "test_framework/generic_test.h"
using std::vector;

unique_ptr<BinaryTreeNode<int>> BinaryTreeFromPreorderInorder(
	const vector<int>& preorder, const vector<int>& inorder) {
	if (preorder.empty()) return nullptr;

	int pivot = preorder[0];
	int i = 0;
	while (inorder[i] != pivot) i++;

	auto &left = BinaryTreeFromPreorderInorder(
		{ preorder.cbegin() + 1, preorder.cbegin() + i + 1 },
		{ inorder.cbegin(), inorder.cbegin() + i });
	auto &right = BinaryTreeFromPreorderInorder(
		{ preorder.cbegin() + i + 1, preorder.cend() },
		{ inorder.cbegin() + i + 1, inorder.cend() });
	return std::make_unique<BinaryTreeNode<int>>(pivot, std::move(left), std::move(right));
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "preorder", "inorder" };
	return GenericTestMain(
		args, "tree_from_preorder_inorder.cc", "tree_from_preorder_inorder.tsv",
		&BinaryTreeFromPreorderInorder, DefaultComparator{}, param_names);
}
