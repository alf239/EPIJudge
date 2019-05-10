#include <string>
#include "test_framework/generic_test.h"

using std::string;

string next(const string &s) {
    if (s.size() == 0) return "1";
    string result = "";
    char digit = ' ';
    int count = 0;
    for (const char &c : s) {
        if (c == digit) {
            count++;
            continue;
        }
        if (count > 0) {
            result.append(std::to_string(count));
            result.push_back(digit);
        }
        digit = c;
        count = 1;
    }
    if (count > 0) {
        result.append(std::to_string(count));
        result.push_back(digit);
    }
    return result;
}

string LookAndSay(int n) {
    string result = "";
    for (int i = 0; i < n; i++) {
        result = next(result);
    }

    return result;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"n"};
    return GenericTestMain(args, "look_and_say.cc", "look_and_say.tsv",
                           &LookAndSay, DefaultComparator{}, param_names);
}
