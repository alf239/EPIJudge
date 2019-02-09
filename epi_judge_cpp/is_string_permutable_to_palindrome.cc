#include <string>
#include <unordered_map>
#include "test_framework/generic_test.h"
using std::string;

bool CanFormPalindrome(const string& s) {
	std::unordered_map<char, int> counters;
	for (const char &c : s) {
		counters[c]++;
	}
	int odd = 0;
	for (const auto &entry : counters) {
		if (entry.second % 2) odd++;
	}
	return odd < 2;
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "s" };
	return GenericTestMain(args, "is_string_permutable_to_palindrome.cc",
		"is_string_permutable_to_palindrome.tsv",
		&CanFormPalindrome, DefaultComparator{}, param_names);
}
