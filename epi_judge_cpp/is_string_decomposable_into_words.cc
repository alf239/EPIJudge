#include <string>
#include <unordered_set>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
using std::string;
using std::unordered_set;
using std::vector;

vector<string> DecomposeIntoDictionaryWords(const string& domain, const unordered_set<string>& dictionary) {
	const int N = domain.length();

	// s decomposable if k-suffix of s is a word and N-k prefix of s is decomposable
	// dp[i][j] = can decompose an i-suffix cutting off a work of length j.
	vector<int> dp(N + 1, -1);
	dp[0] = 0;
	for (int i = 0; i <= N; i++) {
		if (dp[i] == -1) continue;
		for (int j = 1; j <= N - i; j++) {
			if (dictionary.count(domain.substr(i, j)) != 0) dp[i + j] = i;
		}
	}
	if (dp[N] == -1) return {};
	vector<string> result;
	int i = N;
	do {
		result.emplace_back(domain.substr(dp[i], i - dp[i]));
		i = dp[i];
	} while (i > 0);
	return { result.rbegin(), result.rend() };
}
void DecomposeIntoDictionaryWordsWrapper(
	TimedExecutor& executor, const string& domain,
	const unordered_set<string>& dictionary, bool decomposable) {
	vector<string> result = executor.Run(
		[&] { return DecomposeIntoDictionaryWords(domain, dictionary); });
	if (!decomposable) {
		if (!result.empty()) {
			throw TestFailure("domain is not decomposable");
		}
		return;
	}

	if (std::any_of(std::begin(result), std::end(result),
		[&](const std::string& s) { return !dictionary.count(s); })) {
		throw TestFailure("Result uses words not in dictionary");
	}

	if (std::accumulate(std::begin(result), std::end(result), string()) !=
		domain) {
		throw TestFailure("Result is not composed into domain");
	}
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "executor", "domain", "dictionary",
										 "decomposable" };
	return GenericTestMain(args, "is_string_decomposable_into_words.cc",
		"is_string_decomposable_into_words.tsv",
		&DecomposeIntoDictionaryWordsWrapper,
		DefaultComparator{}, param_names);
}
