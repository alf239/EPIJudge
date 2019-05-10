#include <string>
#include <vector>
#include "test_framework/generic_test.h"

using std::string;
using std::vector;

string MajoritySearch(vector<string>::const_iterator stream_begin,
                      const vector<string>::const_iterator stream_end) {

    string candidate = "";
    int count = 0;

    for (auto &i = stream_begin; i != stream_end; i++) {
        const string &word = *i;
        if (word == candidate) count++;
        else {
            count--;
            if (count < 0) {
                candidate = word;
                count = 1;
            }
        }
    }

    return candidate;
}

string MajoritySearchWrapper(const vector<string> &stream) {
    return MajoritySearch(cbegin(stream), cend(stream));
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"stream"};
    return GenericTestMain(args, "majority_element.cc", "majority_element.tsv",
                           &MajoritySearchWrapper, DefaultComparator{},
                           param_names);
}
