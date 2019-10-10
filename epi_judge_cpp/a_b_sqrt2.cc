#include <vector>
#include "test_framework/generic_test.h"
#include <queue>

using std::vector;
using std::priority_queue;
using std::greater;

vector<double> GenerateFirstKABSqrt2(int k) {
	auto root = sqrt(2);
	priority_queue<double, vector<double>, greater<double>> work;
    for (int a = 0; a <= k; a++) {
    	auto lim = (k - a) / root;
    	for (int b = 0; b <= lim; b++) {
    		work.push(a + b * root);
    	}
    }
    vector<double> result;
    for (int i = 0; i < k; i++) {
    	result.push_back(work.top());
    	work.pop();
    }
    return { result.cbegin(), result.cend() };
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"k"};
    return GenericTestMain(args, "a_b_sqrt2.cc", "a_b_sqrt2.tsv",
                           &GenerateFirstKABSqrt2, DefaultComparator{},
                           param_names);
}
