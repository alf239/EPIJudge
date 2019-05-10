#include <string>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <queue>
#include "test_framework/generic_test.h"
#include "string_transformability.h"

using std::string;
using std::unordered_set;
using std::unordered_map;
using std::deque;
using std::pair;

unordered_set<string> keys(const string &s) {
    unordered_set<string> res;
    for (int i = 0; i < s.size(); i++) {
        res.insert(s.substr(0, i) + s.substr(i + 1, s.size() - i - 1));
    }
    return res;
}

bool neighbours(const string &s, const string &t) {
    bool d = false;
    for (int i = 0; i < s.size(); i++) {
        if (s[i] != t[i]) {
            if (d) return false;
            d = true;
        }
    }
    return d;
}

// Uses BFS to find the least steps of transformation.
int TransformString(unordered_set<string> D, const string &s, const string &t) {
    unordered_set<string> seen;
    unordered_map<string, unordered_set<string>> lookup;

    for (const auto &w : D) {
        for (const auto &k : keys(w)) {
            lookup[k].insert(w);
        }
    }

    deque<pair<int, string>> work;
    work.push_back({0, s});
    while (work.size()) {
        auto current = work.front();
        work.pop_front();
        if (seen.count(current.second)) continue;
        seen.insert(current.second);
        if (current.second == t) return current.first;
        for (const auto &k : keys(current.second)) {
            for (const auto &next : lookup[k]) {
                if (neighbours(current.second, next) && !seen.count(next)) {
                    work.push_back({current.first + 1, next});
                }
            }
        }
    }
    return -1;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"D", "s", "t"};
    return GenericTestMain(args, "string_transformability.cc",
                           "string_transformability.tsv", &TransformString,
                           DefaultComparator{}, param_names);
}
