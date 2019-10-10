#include <vector>
#include "test_framework/generic_test.h"
#include <set>

using std::vector;
using std::set;
using std::greater;

struct Number {
    Number(int a, int b) : a(a), b(b), value(a + b * sqrt(2)) {}

    int a, b;
    double value;
};

vector<double> GenerateFirstKABSqrt2(int k) {
    auto root = sqrt(2);
    auto cmp = [](Number left, Number right) { return left.value < right.value; };
    set<Number, decltype(cmp)> work(cmp);
    work.emplace(0, 0);

    vector<double> result;
    for (int i = 0; i < k; i++) {
        auto min = work.cbegin();
        work.emplace(min->a + 1, min->b);
        work.emplace(min->a, min->b + 1);
        work.erase(min);
        result.push_back(min->value);
    }
    return { result.cbegin(), result.cend() };
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"k"};
    return GenericTestMain(args, "a_b_sqrt2.cc", "a_b_sqrt2.tsv",
                           &GenerateFirstKABSqrt2, DefaultComparator{},
                           param_names);
}
