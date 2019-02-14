#include "list_node.h"
#include "test_framework/generic_test.h"
shared_ptr<ListNode<int>> EvenOddMerge(const shared_ptr<ListNode<int>>& L) {
	if (!L || !(L->next) || !(L->next->next)) return L;

	shared_ptr<ListNode<int>> even = L, odd_start = L->next, odd = L->next, nextnext = L->next->next;

	while (nextnext) {
		even->next = nextnext;
		even = even->next;
		odd->next = nextnext->next;
		odd = odd->next;
		if (!odd) break;
		nextnext = nextnext->next->next;
	}
	even->next = odd_start;
	if (odd) odd->next = nullptr;

	return L;
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "L" };
	return GenericTestMain(args, "even_odd_list_merge.cc",
		"even_odd_list_merge.tsv", &EvenOddMerge,
		DefaultComparator{}, param_names);
}
