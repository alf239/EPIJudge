#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/timed_executor.h"

using std::vector;

struct Interval {
    struct Endpoint {
        bool is_closed;
        int val;
    };

    Endpoint left, right;
};

struct EndpointProper {
    Interval::Endpoint point;
    int cardinality;

    boolean operator<(const EndpointProper &other) {
        return point.val < other.point.val;
    }
};

vector<Interval> UnionOfIntervals(const vector<Interval> &intervals) {
    vector<EndpointProper> endpoints;
    for (const auto &interval : intervals) {
        endpoints.push_back({interval.left, 1});
        endpoints.push_back({interval.right, -1});
    }
    std::sort(endpoints.begin(), endpoints.end());

    Interval::Endpoint last_start{};
    int p = 0;

    vector<Interval> result;
    auto i = endpoints.cbegin();
    while (i != endpoints.cend()) {
        EndpointProper pt = {{false, i->point.val}, 0};
        int lowest = p;
        do {
            if (i->point.is_closed) pt.point.is_closed = true;
            pt.cardinality += i->cardinality;
            if (i->cardinality < 0) lowest += i->cardinality;
            i++;
        } while (i != endpoints.cend() && i->point.val == pt.point.val);
        if ((!pt.point.is_closed && p > 0 && lowest == 0) || p + pt.cardinality == 0) {
            result.push_back({last_start, pt.point});
            last_start = pt.point;
        }
        if (p == 0 && pt.cardinality > 0) {
            last_start = pt.point;
        }
        p += pt.cardinality;
    }

    return result;
}

struct FlatInterval {
    int left_val;
    bool left_is_closed;
    int right_val;
    bool right_is_closed;

    FlatInterval(int left_val, bool left_is_closed, int right_val,
                 bool right_is_closed)
            : left_val(left_val),
              left_is_closed(left_is_closed),
              right_val(right_val),
              right_is_closed(right_is_closed) {}

    explicit FlatInterval(Interval in)
            : left_val(in.left.val),
              left_is_closed(in.left.is_closed),
              right_val(in.right.val),
              right_is_closed(in.right.is_closed) {}

    operator Interval() const {
        return {{left_is_closed,  left_val},
                {right_is_closed, right_val}};
    }

    bool operator==(const FlatInterval &rhs) const {
        return std::tie(left_val, left_is_closed, right_val, right_is_closed) ==
               std::tie(rhs.left_val, rhs.left_is_closed, rhs.right_val,
                        rhs.right_is_closed);
    }
};

template<>
struct SerializationTraits<FlatInterval>
        : UserSerTraits<FlatInterval, int, bool, int, bool> {
};

std::ostream &operator<<(std::ostream &out, const FlatInterval &i) {
    return out << (i.left_is_closed ? '<' : '(') << i.left_val << ", "
               << i.right_val << (i.right_is_closed ? '>' : ')');
}

std::vector<FlatInterval> UnionOfIntervalsWrapper(
        TimedExecutor &executor, const std::vector<FlatInterval> &intervals) {
    std::vector<Interval> casted;
    for (const FlatInterval &i : intervals) {
        casted.push_back(static_cast<Interval>(i));
    }

    std::vector<Interval> result =
            executor.Run([&] { return UnionOfIntervals(casted); });

    return {begin(result), end(result)};
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"executor", "intervals"};
    return GenericTestMain(args, "intervals_union.cc", "intervals_union.tsv",
                           &UnionOfIntervalsWrapper, DefaultComparator{},
                           param_names);
}
