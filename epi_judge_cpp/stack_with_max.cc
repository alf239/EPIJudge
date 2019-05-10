#include <stdexcept>
#include <stack>
#include <algorithm>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/test_failure.h"

using std::length_error;

class Stack {
private:
    std::stack<int, std::vector<int>> data;
    std::stack<int, std::vector<int>> maxes;
public:
    bool Empty() const {
        return data.empty();
    }

    int Max() const {
        return maxes.top();
    }

    int Pop() {
        int x = data.top();
        data.pop();
        maxes.pop();
        return x;
    }

    void Push(int x) {
        data.push(x);
        int max = maxes.empty() ? x : std::max(x, maxes.top());
        maxes.push(max);
    }
};

struct StackOp {
    std::string op;
    int argument;
};

template<>
struct SerializationTraits<StackOp> : UserSerTraits<StackOp, std::string, int> {
};

void StackTester(const std::vector<StackOp> &ops) {
    try {
        Stack s;
        for (auto &x : ops) {
            if (x.op == "Stack") {
                continue;
            } else if (x.op == "push") {
                s.Push(x.argument);
            } else if (x.op == "pop") {
                int result = s.Pop();
                if (result != x.argument) {
                    throw TestFailure("Pop: expected " + std::to_string(x.argument) +
                                      ", got " + std::to_string(result));
                }
            } else if (x.op == "max") {
                int result = s.Max();
                if (result != x.argument) {
                    throw TestFailure("Max: expected " + std::to_string(x.argument) +
                                      ", got " + std::to_string(result));
                }
            } else if (x.op == "empty") {
                int result = s.Empty();
                if (result != x.argument) {
                    throw TestFailure("Empty: expected " + std::to_string(x.argument) +
                                      ", got " + std::to_string(result));
                }
            } else {
                throw std::runtime_error("Unsupported stack operation: " + x.op);
            }
        }
    }
    catch (length_error &) {
        throw TestFailure("Unexpected length_error exception");
    }
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"ops"};
    return GenericTestMain(args, "stack_with_max.cc", "stack_with_max.tsv",
                           &StackTester, DefaultComparator{}, param_names);
}
