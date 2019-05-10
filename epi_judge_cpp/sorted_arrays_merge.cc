#include <vector>
#include <queue>
#include <functional>
#include "test_framework/generic_test.h"

using std::vector;

struct jiterator {
    bool operator>(const jiterator &that) {
        return *current > *that.current;
    }

    std::vector<int>::const_iterator current;
    std::vector<int>::const_iterator end;
};

boolean empty(const jiterator &ji) {
    return ji.current == ji.end;
}

vector<int> MergeSortedArrays(const vector<vector<int>> &sorted_arrays) {
    std::priority_queue<jiterator, vector<jiterator>, std::greater<>> work;

    vector<int> result;

    for (const auto &a : sorted_arrays) {
        if (!a.empty()) {
            work.push({a.cbegin(), a.cend()});
        }
    }

    while (!work.empty()) {
        auto it = work.top();
        work.pop();
        result.push_back(*it.current);
        jiterator nxt = {it.current + 1, it.end};
        if (!empty(nxt)) work.push(nxt);
    }

    return result;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"sorted_arrays"};
    return GenericTestMain(args, "sorted_arrays_merge.cc",
                           "sorted_arrays_merge.tsv", &MergeSortedArrays,
                           DefaultComparator{}, param_names);
}
