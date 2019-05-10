#include <vector>
#include <queue>
#include "test_framework/generic_test.h"

using std::vector;
using std::priority_queue;

vector<double> OnlineMedian(vector<int>::const_iterator sequence_begin,
                            const vector<int>::const_iterator &sequence_end) {
    priority_queue<int, vector<int>, std::greater<>> top;
    priority_queue<int, vector<int>, std::less<>> bottom;

    vector<double> result;

    for (auto i = sequence_begin; i != sequence_end; i++) {
        int val = *i;
        if (top.size() > bottom.size()) {
            if (val > top.top()) {
                int t = top.top();
                top.pop();
                top.push(val);
                bottom.push(t);
            } else {
                bottom.push(val);
            }
            result.push_back(0.5 * top.top() + 0.5 * bottom.top());
        } else {
            if (!bottom.empty() && val < bottom.top()) {
                int t = bottom.top();
                bottom.pop();
                bottom.push(val);
                top.push(t);
            } else {
                top.push(val);
            }
            result.push_back(top.top());
        }
    }

    return result;
}

vector<double> OnlineMedianWrapper(const vector<int> &sequence) {
    return OnlineMedian(cbegin(sequence), cend(sequence));
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"sequence"};
    return GenericTestMain(args, "online_median.cc", "online_median.tsv",
                           &OnlineMedianWrapper, DefaultComparator{},
                           param_names);
}
