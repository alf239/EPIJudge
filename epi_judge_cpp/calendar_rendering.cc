#include <vector>
#include <map>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"

using std::vector;

struct Event {
    int start, finish;
};

int FindMaxSimultaneousEvents(const vector<Event> &A) {
    std::map<double, int> ends;
    for (const auto &a : A) {
        ends[a.start] += 1;
        ends[a.finish + 0.1] -= 1;
    }

    int current = 0, max = 0;
    for (const auto &end : ends) {
        current += end.second;
        if (max < current) max = current;
    }
    return max;
}

template<>
struct SerializationTraits<Event> : UserSerTraits<Event, int, int> {
};

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"A"};
    return GenericTestMain(args, "calendar_rendering.cc",
                           "calendar_rendering.tsv", &FindMaxSimultaneousEvents,
                           DefaultComparator{}, param_names);
}
