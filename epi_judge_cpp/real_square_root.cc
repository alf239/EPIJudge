#include "test_framework/generic_test.h"

const double epsilon = 1e-6;

// x = sqrt(a) => x = a / x => guess is halfway betweeen x and a / x

double improve(double a, double x) {
	return 0.5 * (x + a / x);
}

double SquareRoot(double a) {
	if (a == 0.0) return 0.0;

	double guess = a / 2;
	double prev = 2;
	while (std::abs(prev - guess) > epsilon) {
		prev = guess;
		guess = improve(a, guess);
	}
	return guess;
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "x" };
	return GenericTestMain(args, "real_square_root.cc", "real_square_root.tsv",
		&SquareRoot, DefaultComparator{}, param_names);
}
