#include <stdexcept>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"

using std::invalid_argument;
using std::vector;

int FindMissingElement(vector<int>::const_iterator stream_begin,
                       const vector<int>::const_iterator &stream_end) {

    vector<int> counters(65536);

    for (auto i = stream_begin; i != stream_end; i++) {
        counters[(*i >> 16) & 0xffff]++;
    }

    int hi = 0;
    while (counters[hi] == 65536) hi++;

    counters = vector<int>(65536);

    for (auto i = stream_begin; i != stream_end; i++) {
        const int x = *i;
        if ((x >> 16) == hi) counters[x & 0xffff]++;
    }

    int lo = hi ? 0 : 1;
    while (counters[lo]) lo++;

    return (hi << 16) | lo;
}

int FindMissingElementWrapper(const vector<int> &stream) {
    try {
        return FindMissingElement(cbegin(stream), cend(stream));
    }
    catch (invalid_argument &) {
        throw TestFailure("Unexpected no_missing_element exception");
    }
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"stream"};
    return GenericTestMain(args, "absent_value_array.cc",
                           "absent_value_array.tsv", &FindMissingElementWrapper,
                           DefaultComparator{}, param_names);
}
