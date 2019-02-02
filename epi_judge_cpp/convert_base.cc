#include <string>
#include <vector>
#include "test_framework/generic_test.h"
using std::string;
using std::vector;

char digit(int d) {
	return d < 10 ? '0' + d : 'A' + d - 10;
}

string ConvertBase(const string& num_as_string, int b1, int b2) {
	long value = 0;
	long sign = 1;
	for (char c : num_as_string) {
		if (c == '-') sign = -sign;
		else if (c >= '0' && c <= '9') value = (value * b1) + (c - '0');
		else if (c >= 'A' && c <= 'Z') value = (value * b1) + (c - 'A' + 10);
		else if (c >= 'a' && c <= 'z') value = (value * b1) + (c - 'a' + 10);
	}

	vector<char> result;
	if (!value) result.push_back('0');
	while (value) {
		result.push_back(digit(value % b2));
		value /= b2;
	}
	if (sign < 0) result.push_back('-');
	return { result.rbegin(), result.rend() };
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "num_as_string", "b1", "b2" };
	return GenericTestMain(args, "convert_base.cc", "convert_base.tsv",
		&ConvertBase, DefaultComparator{}, param_names);
}
