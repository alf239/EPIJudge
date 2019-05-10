#include <string>
#include <vector>
#include "test_framework/generic_test.h"

using std::string;
using std::vector;

void add(vector<string> *parts, const string &s) {
    if (!s.empty() && s != ".") {
        if (s == "..") {
            parts->pop_back();
        } else {
            parts->emplace_back(s);
        }
    }
}

string ShortestEquivalentPath(const string &path) {
    if (path.empty())
        return path;
    vector<string> parts;
    auto a = path.cbegin();
    for (auto b = path.cbegin(); b != path.cend(); b++) {
        if (*b == '/') {
            string s{a, b};
            add(&parts, s);
            a = b + 1;
        }
    }
    if (a != path.cend()) {
        add(&parts, string{a, path.cend()});
    }

    string result;

    for (auto &part: parts) {
        result.append("/");
        result.append(part);
    }

    return path[0] == '/' ? result : string{result.cbegin() + 1, result.cend()};

}


int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"path"};
    return GenericTestMain(args, "directory_path_normalization.cc",
                           "directory_path_normalization.tsv",
                           &ShortestEquivalentPath, DefaultComparator{},
                           param_names);
}
