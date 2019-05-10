#include "test_framework/generic_test.h"

unsigned long long lookup[] = {
        0b0000, 0b1000, 0b0100, 0b1100,
        0b0010, 0b1010, 0b0110, 0b1110,
        0b0001, 0b1001, 0b0101, 0b1101,
        0b0011, 0b1011, 0b0111, 0b1111};

unsigned long long ReverseBits(unsigned long long x) {
    unsigned long long result = 0;
    for (int i = 0; i < 16; i++) {
        result = (result << 4) | lookup[x & 0xF];
        x >>= 4;
    }
    return result;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"x"};
    return GenericTestMain(args, "reverse_bits.cc", "reverse_bits.tsv",
                           &ReverseBits, DefaultComparator{}, param_names);
}
