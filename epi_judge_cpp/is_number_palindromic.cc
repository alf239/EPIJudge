#include "test_framework/generic_test.h"

bool is_palindrome(int x, int y) {
    if (x < y) return false;
    if (x == y) return true;
    if (x / 10 == y) return true;
    return is_palindrome(x / 10, y * 10 + x % 10);
}


bool IsPalindromeNumber(int x) {
    if (x < 0) return false;
    if (x < 10) return true;
    if (x < 100) return x % 11 == 0;
    if (x % 10 == 0) return false;

    return is_palindrome(x / 10, x % 10);
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"x"};
    return GenericTestMain(args, "is_number_palindromic.cc",
                           "is_number_palindromic.tsv", &IsPalindromeNumber,
                           DefaultComparator{}, param_names);
}
