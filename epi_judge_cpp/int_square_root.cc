#include "test_framework/generic_test.h"

int SquareRoot(int k) {
    if (k <= 1) return k;

    long long a = 1, b = 65536, lk = k;
    while (a + 1 < b) {
        long long c = (a + b) / 2;
        if (c * c == lk) return (int) c;
        if (c * c < lk) a = c; else b = c;
    }
    return (b * b > lk) ? (int) a : (int) b;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"k"};
    return GenericTestMain(args, "int_square_root.cc", "int_square_root.tsv",
                           &SquareRoot, DefaultComparator{}, param_names);
}
