#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

boolean is_zero(const vector<int> &b) {
	return b.empty() || b.size() == 1 && b[0] == 0;
}

int get(const vector<int> &a, int i) {
	return (a.size() > i) ? a[i] : 0;
}

void set(vector<int> *a, int i, int val) {
	while ((a->size() <= i)) a->emplace_back(0);
	(*a)[i] = val;
}

void Add(vector<int> *a, vector<int> b, int exp) {
	int carry = 0;
	for (int i = 0; i < b.size() || carry > 0; i++) {
		int x = carry + get(b, i) + get(*a, i + exp);
		carry = x / 10;
		set(a, i + exp, x % 10);
	}
}

void Scale(vector<int> *a, int k) {
	int carry = 0;
	for (int i = 0; i < (*a).size() || carry > 0; i++) {
		int x = carry + k * get(*a, i);
		carry = x / 10;
		set(a, i, x % 10);
	}
}

vector<int> Multiply(vector<int> num1, vector<int> num2) {
	if (is_zero(num1) || is_zero(num2)) return { 0 };
	bool neg = num1[0] * num2[0] < 0;

	vector<int> A = { num1.rbegin(), num1.rend() };
	A[A.size() - 1] = abs(A[A.size() - 1]);
	vector<int> B = { num2.rbegin(), num2.rend() };
	B[B.size() - 1] = abs(B[B.size() - 1]);

	vector<int> sum;
	int exp = 0;
	for (int b : B) {
		vector<int> a = A;
		Scale(&a, b);
		Add(&sum, a, exp++);
	}
	if (neg) sum[sum.size() - 1] *= -1;
	return { sum.rbegin(), sum.rend() };
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "num1", "num2" };
	return GenericTestMain(args, "int_as_array_multiply.cc",
		"int_as_array_multiply.tsv", &Multiply,
		DefaultComparator{}, param_names);
}
