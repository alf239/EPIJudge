#include <vector>
#include <iostream>
#include "test_framework/generic_test.h"
using std::vector;

void turn(int *dir, int *dx, int *dy, int *nx, int *ny, int *xx, int *xy) {
	*dir = ((*dir) + 1) % 4;
	switch (*dir) {
	case 0:
		*dx = 1; *dy = 0; (*nx)++; return;
	case 1:
		*dx = 0; *dy = 1; (*ny)++;  return;
	case 2:
		*dx = -1; *dy = 0; (*xx)--; return;
	case 3:
		*dx = 0; *dy = -1; (*xy)--; return;
	}
}

vector<int> MatrixInSpiralOrder(vector<vector<int>> square_matrix) {
	int M = square_matrix.size(); // height
	if (!M) return {};
	int N = square_matrix[0].size(); // width
	vector<int> result;
	int x = 0, y = 0, dx = 1, dy = 0, nx = 0, ny = 0, xx = N - 1, xy = M - 1, dir = 0;
//	std::cout << "Start\n";
	for (int i = 0; i < M * N; i++) {
		result.push_back(square_matrix[y][x]);
		if (dir == 0 && x == xx || dir == 1 && y == xy || dir == 2 && x == nx || dir == 3 && y == ny) {
			turn(&dir, &dx, &dy, &nx, &ny, &xx, &xy);
//			std::cout << "<turn\t" << dir << "\t" << nx << " to " << xx << "\t" << ny << " to " << xy << std::endl;
		}

		x += dx;
		y += dy;
	}
	return result;
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "square_matrix" };
	return GenericTestMain(args, "spiral_ordering_segments.cc",
		"spiral_ordering_segments.tsv", &MatrixInSpiralOrder,
		DefaultComparator{}, param_names);
}
