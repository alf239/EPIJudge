#include "list_node.h"
#include "test_framework/generic_test.h"

shared_ptr<ListNode<int>> EvenOddMerge(const shared_ptr<ListNode<int>> &L) {
    if (!L || !(L->next)) return L;

    bool even = true;
    shared_ptr<ListNode<int>> a = L, b = L->next, next = L->next->next, odd_start = L->next;

    while (next) {
        auto &grow = even ? a : b;
        grow->next = next;
        grow = grow->next;
        next = next->next;
        even = !even;
    }
    a->next = odd_start;
    b->next = nullptr;

    return L;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"L"};
    return GenericTestMain(args, "even_odd_list_merge.cc",
                           "even_odd_list_merge.tsv", &EvenOddMerge,
                           DefaultComparator{}, param_names);
}
