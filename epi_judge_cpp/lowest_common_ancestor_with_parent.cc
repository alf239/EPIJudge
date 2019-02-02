#include "binary_tree_with_parent_prototype.h"
#include "test_framework/binary_tree_utils.h"
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
#include<vector>
using std::vector;

BinaryTreeNode<int>* LCA(const unique_ptr<BinaryTreeNode<int>>& node0,
	const unique_ptr<BinaryTreeNode<int>>& node1) {

	if (!node0 || !node1) return nullptr;
	if (node0 == node1) return node0.get();

	vector<BinaryTreeNode<int>*> parents0, parents1;
	BinaryTreeNode<int> *a = node0.get();
	BinaryTreeNode<int> *b = node1.get();

	while(a) {
		parents0.push_back(a);
		a = a->parent;
	}
	while (b) {
		parents1.push_back(b);
		b = b->parent;
	}

	auto i = parents0.rbegin();
	auto j = parents1.rbegin();

	auto parent = a;
	while (i != parents0.rend() && j != parents1.rend() && *i == *j) {
		parent = *i;
		i++; j++;
	}

	return parent;
}
int LcaWrapper(TimedExecutor& executor,
	const unique_ptr<BinaryTreeNode<int>>& tree, int key0,
	int key1) {
	const unique_ptr<BinaryTreeNode<int>>& node0 = MustFindNode(tree, key0);
	const unique_ptr<BinaryTreeNode<int>>& node1 = MustFindNode(tree, key1);

	auto result = executor.Run([&] { return LCA(node0, node1); });

	if (!result) {
		throw TestFailure("Result can not be nullptr");
	}
	return result->data;
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "executor", "tree", "key0", "key1" };
	return GenericTestMain(args, "lowest_common_ancestor_with_parent.cc",
		"lowest_common_ancestor.tsv", &LcaWrapper,
		DefaultComparator{}, param_names);
}
