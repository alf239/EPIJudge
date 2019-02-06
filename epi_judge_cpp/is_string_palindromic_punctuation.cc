#include <string>
#include <cctype>
#include "test_framework/generic_test.h"
using std::string;

bool letter(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

bool same(char a, char b) {
	return tolower(a) == tolower(b);
}

bool IsPalindrome(const string& s) {
	if (s.size() <= 1) return true;

	bool seen_a_letter = false;
	int i = 0, j = s.size() - 1;
	while (i < j) {
		while (i < j && !letter(s[i])) i++;
		while (i < j && !letter(s[j])) j--;
		if (i < j && !same(s[i++], s[j--])) return false;
		seen_a_letter = true;
	}
	return seen_a_letter || (i == j) && letter(s[i]);
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "s" };
	return GenericTestMain(args, "is_string_palindromic_punctuation.cc",
		"is_string_palindromic_punctuation.tsv", &IsPalindrome,
		DefaultComparator{}, param_names);
}
