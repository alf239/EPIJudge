#include <string>
#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"

using std::string;

void reverse(string *s, int i, int j) {
    while (i < j) {
        std::swap((*s)[i++], (*s)[j--]);
    }
}

void ReverseWords(string *s) {
    int i, j = 0;
    int N = s->size();
    while (j < N) {
        i = j;
        while (j < N && (*s)[j] != ' ') j++;
        reverse(s, i, j - 1);
        j++;
    }
    reverse(s, 0, N - 1);
}

string ReverseWordsWrapper(TimedExecutor &executor, string s) {
    string s_copy = s;

    executor.Run([&] { ReverseWords(&s_copy); });

    return s_copy;
}

int main(int argc, char *argv[]) {
    std::vector<std::string> args{argv + 1, argv + argc};
    std::vector<std::string> param_names{"executor", "s"};
    return GenericTestMain(args, "reverse_words.cc", "reverse_words.tsv",
                           &ReverseWordsWrapper, DefaultComparator{},
                           param_names);
}
