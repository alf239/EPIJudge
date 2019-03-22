#include <memory>
#include <vector>
#include "bst_node.h"
#include "test_framework/binary_tree_utils.h"
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
using std::unique_ptr;
using std::vector;

unique_ptr<BstNode<int>> build(const vector<int>& A, int from, int to) {
	if (from > to) return nullptr;
	int pivot = (from + to) >> 1;
	return unique_ptr<BstNode<int>>(new BstNode<int>(A[pivot],
		std::move(build(A, from, pivot - 1)),
		std::move(build(A, pivot + 1, to))));
}

unique_ptr<BstNode<int>> BuildMinHeightBSTFromSortedArray(const vector<int>& A) {
	return A.empty() ? nullptr : build(A, 0, A.size() - 1);
}

int BuildMinHeightBSTFromSortedArrayWrapper(TimedExecutor& executor,
	const vector<int>& A) {
	unique_ptr<BstNode<int>> result =
		executor.Run([&] { return BuildMinHeightBSTFromSortedArray(A); });

	if (GenerateInorder(result) != A) {
		throw TestFailure("Result binary tree mismatches input array");
	}
	return BinaryTreeHeight(result);
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "executor", "A" };
	return GenericTestMain(args, "bst_from_sorted_array.cc",
		"bst_from_sorted_array.tsv",
		&BuildMinHeightBSTFromSortedArrayWrapper,
		DefaultComparator{}, param_names);
}
