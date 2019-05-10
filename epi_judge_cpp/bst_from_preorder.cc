#include <memory>
#include <limits>
#include "bst_node.h"
#include "test_framework/generic_test.h"

using std::unique_ptr;
using std::vector;

unique_ptr<BstNode<int>> rebuild(const vector<int> &seq, int *i, const long long cutoff) {
    if (*i >= seq.size()) return nullptr;
    int data = seq[*i];
    if (data > cutoff) return nullptr;
    (*i)++;
    unique_ptr<BstNode<int>> left = std::move(rebuild(seq, i, data));
    unique_ptr<BstNode<int>> right = std::move(rebuild(seq, i, cutoff));
    return unique_ptr<BstNode<int>>(new BstNode<int>(data, std::move(left), std::move(right)));
}


unique_ptr<BstNode<int>> RebuildBSTFromPreorder(const vector<int> &preorder_sequence) {
    int i = 0;
    auto result = rebuild(preorder_sequence, &i, std::numeric_limits<long long>::max());
    return result;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"preorder_sequence"};
    return GenericTestMain(args, "bst_from_preorder.cc", "bst_from_preorder.tsv",
                           &RebuildBSTFromPreorder, DefaultComparator{},
                           param_names);
}
