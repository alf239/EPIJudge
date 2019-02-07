#include <memory>
#include <algorithm>
#include "list_node.h"
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
using std::shared_ptr;

std::pair<int, shared_ptr<ListNode<int>>> length_and_last(shared_ptr<ListNode<int>> l) {
	shared_ptr<ListNode<int>> prev = nullptr;
	int i = 0;
	while (l) {
		i++;
		prev = l;
		l = l->next;
	}
	return { i, prev };
}

shared_ptr<ListNode<int>> drop(int n, shared_ptr<ListNode<int>> l) {
	for (int i = 0; i < n; i++) l = l->next;
	return l;
}

shared_ptr<ListNode<int>> OverlappingNoCycleLists(
	shared_ptr<ListNode<int>> l0, shared_ptr<ListNode<int>> l1) {
	if (!l0 || !l1) return nullptr;

	auto lnl0 = length_and_last(l0);
	auto lnl1 = length_and_last(l1);
	if (lnl0.second != lnl1.second) return nullptr;

	if (lnl0.first > lnl1.first)
		l0 = drop(lnl0.first - lnl1.first, l0);
	else
		l1 = drop(lnl1.first - lnl0.first, l1);

	while (l0 != l1) {
		l0 = l0->next;
		l1 = l1->next;
	}
	return l0;
}

void OverlappingNoCycleListsWrapper(TimedExecutor& executor,
	shared_ptr<ListNode<int>> l0,
	shared_ptr<ListNode<int>> l1,
	shared_ptr<ListNode<int>> common) {
	if (common) {
		if (l0) {
			auto i = l0;
			while (i->next) {
				i = i->next;
			}
			i->next = common;
		}
		else {
			l0 = common;
		}

		if (l1) {
			auto i = l1;
			while (i->next) {
				i = i->next;
			}
			i->next = common;
		}
		else {
			l1 = common;
		}
	}

	auto result = executor.Run([&] { return OverlappingNoCycleLists(l0, l1); });

	if (result != common) {
		throw TestFailure("Invalid result");
	}
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "executor", "l0", "l1", "common" };
	return GenericTestMain(
		args, "do_terminated_lists_overlap.cc", "do_terminated_lists_overlap.tsv",
		&OverlappingNoCycleListsWrapper, DefaultComparator{}, param_names);
}
