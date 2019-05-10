#include <string>
#include <vector>
#include <limits>
#include <unordered_map>
#include "test_framework/generic_test.h"

using std::string;
using std::vector;
using std::unordered_map;


int FindNearestRepetition(const vector<string> &paragraph) {
    int closest = std::numeric_limits<int>::max();
    unordered_map<string, int> last;

    int i = 0;
    for (auto s : paragraph) {
        i++;
        int &seen = last[s];
        if (seen != 0 && (i - seen) < closest) closest = i - seen;
        seen = i;
    }

    return closest == std::numeric_limits<int>::max() ? -1 : closest;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"paragraph"};
    return GenericTestMain(args, "nearest_repeated_entries.cc",
                           "nearest_repeated_entries.tsv", &FindNearestRepetition,
                           DefaultComparator{}, param_names);
}
