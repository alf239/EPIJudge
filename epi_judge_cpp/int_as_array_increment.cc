#include <vector>
#include "test_framework/generic_test.h"

using std::vector;

vector<int> PlusOne(vector<int> A) {
    vector<int> res;
    int carry = 1;
    for (int i = A.size(); --i >= 0;) {
        int t = A[i] + carry;
        res.push_back(t % 10);
        carry = t / 10;
    }
    if (carry) {
        res.push_back(carry);
    }
    return {res.rbegin(), res.rend()};
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"A"};
    return GenericTestMain(args, "int_as_array_increment.cc",
                           "int_as_array_increment.tsv", &PlusOne,
                           DefaultComparator{}, param_names);
}
