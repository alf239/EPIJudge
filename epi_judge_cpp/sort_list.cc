#include <memory>
#include <list>
#include "list_node.h"
#include "test_framework/generic_test.h"
#include "sort_list.h"

using std::list;

list<shared_ptr<ListNode<int>>> as_lol(shared_ptr<ListNode<int>> a) {
	list<shared_ptr<ListNode<int>>> result;
	while (a) {
		result.emplace_back(make_shared<ListNode<int>>(a->data));
		a = a->next;
	}
	return result;
}

shared_ptr<ListNode<int>> merge(shared_ptr<ListNode<int>> a, shared_ptr<ListNode<int>> b) {
	auto result = make_shared<ListNode<int>>(-1);
	auto end = result;
	
	do {
		if (a->data > b->data) std::swap(a, b);
		end->next = a;
		end = end->next;
		a = a->next;
	} while (a);
	end->next = b;

	return result->next;
}

void merge_pairs(list<shared_ptr<ListNode<int>>> *lol) {
	auto c = lol->begin();
	auto e = lol->end();
	while (c != e) {
		auto a = c++;
		if (c == e) break;
		auto b = c++;
		*a = merge(*a, *b);
		lol->erase(b, c);
	}
}

shared_ptr<ListNode<int>> StableSortList(shared_ptr<ListNode<int>> L) {
	if (!L) return L;
	if (!L->next) return L;

	list<shared_ptr<ListNode<int>>> &lol = as_lol(L);
	while (lol.size() > 1) {
		merge_pairs(&lol);
	}
	return lol.front();
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "L" };
	return GenericTestMain(args, "sort_list.cc", "sort_list.tsv", &StableSortList,
		DefaultComparator{}, param_names);
}
