#include <vector>
#include <queue>
#include "test_framework/generic_test.h"

using std::vector;
using std::queue;
using std::priority_queue;

struct Work {
    int i, value;
};

vector<int> KLargestInBinaryHeap(const vector<int> &A, int k) {
    unsigned long long int N = A.size();
    if (k >= N) return A;

    auto cmp = [](Work left, Work right) { return left.value < right.value; };
    priority_queue<Work, vector<Work>, decltype(cmp)> work(cmp);
    work.emplace(Work{0, A[0]});

    vector<int> result;
    while (result.size() < k) {
        Work w = work.top();
        work.pop();
        result.emplace_back(w.value);
        int next = w.i * 2 + 1;
        if (next < N) {
            work.emplace(Work{next, A[next]});
            if (next + 1 < N) {
                work.emplace(Work{next + 1, A[next + 1]});
            }
        }
    }

    return result;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"A", "k"};
    return GenericTestMain(args, "k_largest_in_heap.cc", "k_largest_in_heap.tsv",
                           &KLargestInBinaryHeap,
                           &UnorderedComparator<std::vector<int>>, param_names);
}
