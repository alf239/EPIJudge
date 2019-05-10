#include <string>
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"

using std::string;

string IntToString(int x) {
    if (x == 0) return "0";

    string res = "";
    auto xx = x;
    while (xx) {
        res.push_back('0' + std::abs(xx % 10));
        xx /= 10;
    }
    if (x < 0) res.push_back('-');
    std::reverse(res.begin(), res.end());
    return res;
}

int StringToInt(const string &s) {
    long res = 0;
    int sign = 1;
    for (auto c : s) {
        if (c == '-') sign = -1;
        else res = res * 10 + (c - '0');
    }
    return (int) (res * sign);
}

void Wrapper(int x, const string &s) {
    if (IntToString(x) != s) {
        throw TestFailure("Int to string conversion failed");
    }

    if (StringToInt(s) != x) {
        throw TestFailure("String to int conversion failed");
    }
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"x", "s"};
    return GenericTestMain(args, "string_integer_interconversion.cc",
                           "string_integer_interconversion.tsv", &Wrapper,
                           DefaultComparator{}, param_names);
}
