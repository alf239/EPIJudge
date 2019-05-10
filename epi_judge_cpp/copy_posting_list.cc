#include <map>
#include <memory>
#include "posting_list_node.h"
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"

using std::make_shared;
using std::shared_ptr;

shared_ptr<PostingListNode> CopyPostingsList(
        const shared_ptr<PostingListNode> &l) {
    // TODO - you fill in here.
    return nullptr;
}

using PostingListPtr = std::shared_ptr<PostingListNode>;

struct SerializedNode {
    int order;
    int jump_index;
};

template<>
struct SerializationTraits<SerializedNode>
        : UserSerTraits<SerializedNode, int, int> {
};

PostingListPtr CreatePostingList(
        const std::vector<SerializedNode> &serialized) {
    std::map<int, PostingListPtr> key_mapping;
    PostingListPtr head;
    for (auto it = rbegin(serialized); it != rend(serialized); ++it) {
        head = make_shared<PostingListNode>(it->order, head, nullptr);
        key_mapping[it->order] = head;
    }
    auto list_it = head;
    for (auto it = begin(serialized); it != end(serialized);
         ++it, list_it = list_it->next) {
        if (it->jump_index != -1) {
            list_it->jump = key_mapping[it->jump_index].get();
            if (!list_it->jump) throw std::runtime_error("Jump index out of range");
        }
    }

    return head;
}

void AssertListsEqual(const PostingListPtr &orig, const PostingListPtr &copy) {
    std::map<PostingListNode *, PostingListNode *> node_mapping;
    auto o_it = orig;
    auto c_it = copy;
    while (o_it) {
        if (!c_it) {
            throw TestFailure("Copied list has fewer nodes than the original");
        }
        if (o_it->order != c_it->order) {
            throw TestFailure("Order value mismatch");
        }
        node_mapping[o_it.get()] = c_it.get();
        o_it = o_it->next;
        c_it = c_it->next;
    }

    if (c_it) {
        throw TestFailure("Copied list has more nodes than the original");
    }

    o_it = orig;
    c_it = copy;
    while (o_it) {
        if (node_mapping.count(c_it.get())) {
            throw TestFailure("Copied list contains a node from the original list");
        }
        if (node_mapping[o_it->jump] != c_it->jump) {
            throw TestFailure(
                    "Jump link points to a different nodes in the copied list");
        }
        o_it = o_it->next;
        c_it = c_it->next;
    }
}

void CopyPostingsListWrapper(TimedExecutor &executor,
                             const std::vector<SerializedNode> &l) {
    auto head = CreatePostingList(l);

    auto copy = executor.Run([&] { return CopyPostingsList(head); });

    AssertListsEqual(head, copy);
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"executor", "l"};
    return GenericTestMain(args, "copy_posting_list.cc", "copy_posting_list.tsv",
                           &CopyPostingsListWrapper, DefaultComparator{},
                           param_names);
}
