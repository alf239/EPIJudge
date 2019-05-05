#include <utility>

#include "list_node.h"
#include "test_framework/generic_test.h"

int size(shared_ptr<ListNode<int>> L) {
    int result = 0;
    while (L != nullptr) {
        result++;
        L = L->next;
    }
    return result;
}

shared_ptr<ListNode<int>> reverse(shared_ptr<ListNode<int>> L, shared_ptr<ListNode<int>> tail) {
    while (L != nullptr) {
        auto t = L->next;
        L->next = tail;
        tail = L;
        L = t;
    }
    return tail;
}

shared_ptr<ListNode<int>> drop(int n, shared_ptr<ListNode<int>> L) {
    while (n-- > 0) {
        L = L->next;
    }
    return L;
}

bool samePrefix(shared_ptr<ListNode<int>> A, shared_ptr<ListNode<int>> B) {
    while (A != nullptr && B != nullptr) {
        if (A->data != B->data) return false;
        A = A->next;
        B = B->next;
    }
    return true;
}

bool IsLinkedListAPalindrome(const shared_ptr<ListNode<int>> &L) {
    int n = size(L);
    if (n <= 1) return true;

    int half = (n + 1) / 2;
    auto liat = reverse(drop(half, L), nullptr);
    auto result = samePrefix(liat, L);
    reverse(liat, nullptr);
    return result;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"L"};
    return GenericTestMain(args, "is_list_palindromic.cc",
                           "is_list_palindromic.tsv", &IsLinkedListAPalindrome,
                           DefaultComparator{}, param_names);
}
