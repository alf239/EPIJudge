#include "test_framework/generic_test.h"
double Power(double x, int y) {
	bool flip = y < 0;
	if (flip) y = -y;
	double result = 1.0;
	while (y) {
		if (y % 2) result *= x;
		y >>= 1;
		x = x * x;
	}
	return flip ? 1.0 / result : result;
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "x", "y" };
	return GenericTestMain(args, "power_x_y.cc", "power_x_y.tsv", &Power,
		DefaultComparator{}, param_names);
}
