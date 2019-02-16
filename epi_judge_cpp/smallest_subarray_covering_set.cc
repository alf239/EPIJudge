#include <string>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
using std::string;
using std::unordered_set;
using std::unordered_map;
using std::vector;

struct Subarray {
	int start, end;
};

int len(Subarray s) {
	return s.end - s.start;
}

Subarray FindSmallestSubarrayCoveringSet(
	const vector<string> &paragraph, const unordered_set<string> &keywords) {

	unordered_map<string, int> in_the_window;
	Subarray current = { 0, 0 };

	unordered_set<string> missing = keywords;
	while (current.end < paragraph.size() && !missing.empty()) {
		const auto &word = paragraph[current.end++];
		if (keywords.count(word)) {
			missing.erase(word);
			in_the_window[word]++;
		}
	}
	if (!missing.empty()) return { -1, -1 };

	while (current.start < paragraph.size() &&
		(keywords.count(paragraph[current.start]) == 0 ||
			in_the_window[paragraph[current.start]] > 1)) {
		in_the_window[paragraph[current.start++]] -= 1;
	}

	Subarray smallest = current;
	while (current.end < paragraph.size()) {
		while (current.end < paragraph.size() && keywords.count(paragraph[current.end]) == 0) {
			current.end++;
		}
		if (current.end < paragraph.size()) {
			in_the_window[paragraph[current.end++]] += 1;
			while (keywords.count(paragraph[current.start]) == 0 ||
				in_the_window[paragraph[current.start]] > 1) {
				in_the_window[paragraph[current.start++]] -= 1;
			}
			if (len(current) < len(smallest)) smallest = current;
		}
	}
	smallest.end--;
	return smallest;
}
int FindSmallestSubarrayCoveringSetWrapper(
	TimedExecutor &executor, const vector<string> &paragraph,
	const unordered_set<string> &keywords) {
	unordered_set<string> copy = keywords;

	auto result = executor.Run(
		[&] { return FindSmallestSubarrayCoveringSet(paragraph, keywords); });

	if (result.start < 0 || result.start >= paragraph.size() || result.end < 0 ||
		result.end >= paragraph.size() || result.start > result.end) {
		throw TestFailure("Index out of range");
	}

	for (int i = result.start; i <= result.end; i++) {
		copy.erase(paragraph[i]);
	}

	if (!copy.empty()) {
		throw TestFailure("Not all keywords are in the range");
	}

	return result.end - result.start + 1;
}

int main(int argc, char *argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "executor", "paragraph", "keywords" };
	return GenericTestMain(args, "smallest_subarray_covering_set.cc",
		"smallest_subarray_covering_set.tsv",
		&FindSmallestSubarrayCoveringSetWrapper,
		DefaultComparator{}, param_names);
}
