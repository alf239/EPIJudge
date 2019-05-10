#include <string>
#include "test_framework/generic_test.h"

using std::string;

int value(char c) {
    switch (c) {
        case 'I':
            return 1;
        case 'V':
            return 5;
        case 'X':
            return 10;
        case 'L':
            return 50;
        case 'C':
            return 100;
        case 'D':
            return 500;
        case 'M':
            return 1000;
        default:
            return -1;
    }
}

int RomanToInteger(const string &s) {
    int last = 0;
    int result = 0;
    for (const char c : s) {
        int v = value(c);
        int adjusted_val = v <= last ? v : v - (2 * last);
        last = v;
        result += adjusted_val;
    }
    return result;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"s"};
    return GenericTestMain(args, "roman_to_integer.cc", "roman_to_integer.tsv",
                           &RomanToInteger, DefaultComparator{}, param_names);
}
