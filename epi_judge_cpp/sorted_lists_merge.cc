#include "list_node.h"
#include "test_framework/generic_test.h"


void Append(shared_ptr<ListNode<int>> *tail,
	shared_ptr<ListNode<int>> *node) {
	(*tail)->next = *node;
	*tail = *node;
	*node = (*node)->next;
}

shared_ptr<ListNode<int>> MergeTwoSortedLists(shared_ptr<ListNode<int>> L1,
	shared_ptr<ListNode<int>> L2) {
	if (!L1) return L2;
	if (!L2) return L1;

	shared_ptr<ListNode<int>> res(new ListNode<int>());
	shared_ptr<ListNode<int>> tail = res;


	while (L1 && L2) {
		if (L1->data > L2->data) Append(&tail, &L2);
		else Append(&tail, &L1);
	}

	tail->next = L1 ? L1 : L2;

	return res->next;
}


int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "L1", "L2" };
	return GenericTestMain(args, "sorted_lists_merge.cc",
		"sorted_lists_merge.tsv", &MergeTwoSortedLists,
		DefaultComparator{}, param_names);
}
