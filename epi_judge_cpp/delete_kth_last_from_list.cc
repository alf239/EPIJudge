#include <memory>
#include "list_node.h"
#include "test_framework/generic_test.h"

using std::shared_ptr;

ListNode<int> *drop(int n, ListNode<int> *L) {
    for (int i = 0; i < n; i++) L = L->next.get();
    return L;
}

// Assumes L has at least k nodes, deletes the k-th last node in L.
shared_ptr<ListNode<int>> RemoveKthLast(const shared_ptr<ListNode<int>> &L,
                                        int k) {
    ListNode<int> dummy{-1, L};
    ListNode<int> *prev = &dummy, *front = drop(k, L.get());
    while (front) {
        prev = prev->next.get();
        front = front->next.get();
    }
    prev->next = prev->next->next;
    return dummy.next;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"L", "k"};
    return GenericTestMain(args, "delete_kth_last_from_list.cc",
                           "delete_kth_last_from_list.tsv", &RemoveKthLast,
                           DefaultComparator{}, param_names);
}
