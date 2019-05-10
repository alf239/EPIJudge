#include <string>
#include <stack>
#include "test_framework/generic_test.h"

using std::string;
using std::stack;

bool IsWellFormed(const string &s) {
    const string opening = "([{";
    const string closing = ")]}";
    stack<int> wrk;
    for (const auto &c : s) {
        int open = opening.find(c);
        if (open != string::npos) wrk.push(open);
        else {
            int close = closing.find(c);
            if (close != string::npos) {
                if (wrk.empty() || wrk.top() != close) return false;
                wrk.pop();
            }
        }
    }
    return wrk.empty();
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"s"};
    return GenericTestMain(args, "is_valid_parenthesization.cc",
                           "is_valid_parenthesization.tsv", &IsWellFormed,
                           DefaultComparator{}, param_names);
}
