#include <vector>
#include "test_framework/generic_test.h"

using std::vector;

vector<vector<int>> GeneratePowerSet(const vector<int> &input_set) {
    vector<vector<int>> out;
    out.emplace_back(vector<int>());
    for (auto x : input_set) {
        int snapped_size = out.size();
        for (int i = 0; i < snapped_size; i++) {
            auto v = out[i]; // copy!
            v.emplace_back(x);
            out.emplace_back(v);
        }
    }
    return out;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"input_set"};
    return GenericTestMain(
            args, "power_set.cc", "power_set.tsv", &GeneratePowerSet,
            &UnorderedComparator<vector<vector<int>>>, param_names);
}
