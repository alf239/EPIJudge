#include <string>
#include <queue>
#include "test_framework/generic_test.h"
using std::string;

void execute(const string& command, std::deque<int> *stack) {
	char last = command[command.size() - 1];
	if (last >= '0' && last <= '9') {
		stack->push_back(std::stoi(command));
	}
	else {
		int b = stack->back();
		stack->pop_back();
		int a = stack->back();
		stack->pop_back();
		switch (command[0]) {
		case '+': stack->push_back(a + b);
			break;
		case '-': stack->push_back(a - b);
			break;
		case '*': stack->push_back(a * b);
			break;
		case '/': stack->push_back(a / b);
			break;
		}
	}
}

int Evaluate(const string& expression) {
	std::deque<int> stack;
	auto i = expression.cbegin();
	auto end = expression.cend();

	while (i != end) {
		auto j = i;
		while (i != end && *i != ',') i++;
		execute({ j, i }, &stack);
		if (i != end) i++;
	}

	return stack.back();
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "expression" };
	return GenericTestMain(args, "evaluate_rpn.cc", "evaluate_rpn.tsv", &Evaluate,
		DefaultComparator{}, param_names);
}
