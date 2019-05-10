#include <istream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_set>
#include <memory>
#include <iostream>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"

using std::vector;
typedef enum {
    kWhite, kBlack
} Color;

struct Coordinate {
    bool operator==(const Coordinate &that) const {
        return x == that.x && y == that.y;
    }

    int x, y;
};

// custom specialization of std::hash can be injected in namespace std
struct CoordHash {
    std::size_t operator()(Coordinate const &s) const noexcept {
        return s.x + 37 * (s.y + 17);
    }
};

// Bartosz Milewski's cons-List in C++ - START
// see https://github.com/BartoszMilewski/Okasaki/blob/master/SimpleList
template<class T>
class List {
    struct Item {
        Item(T v, Item const *tail) : _val(v), _next(tail) {}

        T _val;
        Item const *_next;
    };

    friend Item;

    explicit List(Item const *items) : _head(items) {}

public:
    // Empty list
    List() : _head(nullptr) {}

    // Cons
    List(T v, List tail) : _head(new Item(v, tail._head)) {}

    // From initializer list
    List(std::initializer_list<T> init) : _head(nullptr) {
        for (auto it = std::rbegin(init); it != std::rend(init); ++it) {
            _head = new Item(*it, _head);
        }
    }

    bool isEmpty() const { return !_head; }

    T front() const {
        assert(!isEmpty());
        return _head->_val;
    }

    List popped_front() const {
        assert(!isEmpty());
        return List(_head->_next);
    }

    // Additional utilities
    List pushed_front(T v) const {
        return List(v, *this);
    }

    List insertedAt(int i, T v) const {
        if (i == 0)
            return pushed_front(v);
        else {
            assert(!isEmpty());
            return List(front(), popped_front().insertedAt(i - 1, v));
        }
    }

private:
    // Old C++ trick to encode a Maybe value
    Item const *_head;
};

template<class T, class F>
void forEach(List<T> lst, F f) {
    static_assert(std::is_convertible<F, std::function<void(T)>>::value,
                  "forEach requires a function type void(T)");
    if (!lst.isEmpty()) {
        f(lst.front());
        forEach(lst.popped_front(), f);
    }
}
// Bartosz Milewski's cons-List in C++ - END


struct Step {
    bool operator<(const Step &that) const {
        return dist < that.dist;
    }

    Coordinate s;
    long dist;
    List<Coordinate> prev;
};


bool step_order(const Step &a, const Step &b) {
    return a.dist > b.dist;
}

long sqr(long x) {
    return x * x;
}

long distance(const Coordinate &s, const Coordinate &e) {
    return sqr(s.x - e.x) + sqr(s.y - e.y);
}

vector<Coordinate> breadcrumbs(const Step &finish) {
    vector<Coordinate> result;
    result.push_back(finish.s);
    forEach(finish.prev, [&result](const Coordinate &c) { result.push_back(c); });
    return {result.rbegin(), result.rend()};
}

bool PathElementIsFeasible(const vector<vector<Color>> &maze,
                           const Coordinate &prev, const Coordinate &cur);

vector<Coordinate> SearchMaze(vector<vector<Color>> maze, const Coordinate &s,
                              const Coordinate &e) {
    std::priority_queue<Step, vector<Step>, decltype(&step_order)> work(step_order);
    std::unordered_set<Coordinate, CoordHash> seen;

    work.push({s, distance(s, e), List<Coordinate>()});

    while (work.size()) {
        Step step = work.top();
        work.pop();

        Coordinate c = step.s;

        if (seen.count(c)) continue;

        if (c == e) return breadcrumbs(step);

        seen.emplace(c);

        vector<Coordinate> next = {
                {c.x - 1, c.y},
                {c.x + 1, c.y},
                {c.x,     c.y - 1},
                {c.x,     c.y + 1}
        };

        auto path = step.prev.pushed_front(c);

        for (Coordinate cc : next) {
            if (PathElementIsFeasible(maze, c, cc) && !seen.count(cc)) {
                Step s = {cc, distance(cc, e), path};
                work.push(s);
            }
        }
    }

    return {};
}

template<>
struct SerializationTraits<Color> : SerializationTraits<int> {
    using serialization_type = Color;

    static serialization_type Parse(const std::string &str) {
        return static_cast<serialization_type>(
                SerializationTraits<int>::Parse(str));
    }

    static serialization_type JsonParse(const json_parser::Json &json_object) {
        return static_cast<serialization_type>(
                SerializationTraits<int>::JsonParse(json_object));
    }
};

template<>
struct SerializationTraits<Coordinate> : UserSerTraits<Coordinate, int, int> {
    static std::vector<std::string> GetMetricNames(const std::string &arg_name) {
        return {};
    }

    static std::vector<int> GetMetrics(const Coordinate &x) { return {}; }
};

bool PathElementIsFeasible(const vector<vector<Color>> &maze,
                           const Coordinate &prev, const Coordinate &cur) {
    if (!(0 <= cur.x && cur.x < maze.size() && 0 <= cur.y &&
          cur.y < maze[cur.x].size() && maze[cur.x][cur.y] == kWhite)) {
        return false;
    }
    return cur == Coordinate{prev.x + 1, prev.y} ||
           cur == Coordinate{prev.x - 1, prev.y} ||
           cur == Coordinate{prev.x, prev.y + 1} ||
           cur == Coordinate{prev.x, prev.y - 1};
}

bool SearchMazeWrapper(TimedExecutor &executor,
                       const vector<vector<Color>> &maze, const Coordinate &s,
                       const Coordinate &e) {
    vector<vector<Color>> copy = maze;

    auto path = executor.Run([&] { return SearchMaze(copy, s, e); });

    if (path.empty()) {
        return s == e;
    }

    if (!(path.front() == s) || !(path.back() == e)) {
        throw TestFailure("Path doesn't lay between start and end points");
    }

    for (size_t i = 1; i < path.size(); i++) {
        if (!PathElementIsFeasible(maze, path[i - 1], path[i])) {
            throw TestFailure("Path contains invalid segments");
        }
    }

    return true;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"executor", "maze", "s", "e"};
    return GenericTestMain(args, "search_maze.cc", "search_maze.tsv",
                           &SearchMazeWrapper, DefaultComparator{}, param_names);
}
