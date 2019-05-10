#include "list_node.h"
#include "test_framework/generic_test.h"

shared_ptr<ListNode<int>> ReverseSublist(shared_ptr<ListNode<int>> L, int start,
                                         int finish) {

    if (start == finish) return L;

    ListNode<int> dummy(-1, L);

    ListNode<int> *lastFront = &dummy;
    for (int i = 1; i < start; i++) {
        lastFront = lastFront->next.get();
    }
    ListNode<int> *lastSegment = lastFront->next.get();
    auto prev = lastFront->next;
    auto current = lastFront->next->next;

    for (int i = start + 1; i <= finish; i++) {
        shared_ptr<ListNode<int>> next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    lastFront->next = prev;
    lastSegment->next = current;

    return dummy.next;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"L", "start", "finish"};
    return GenericTestMain(args, "reverse_sublist.cc", "reverse_sublist.tsv",
                           &ReverseSublist, DefaultComparator{}, param_names);
}
