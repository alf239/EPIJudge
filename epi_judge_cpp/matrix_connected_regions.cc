#include <deque>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/timed_executor.h"
using std::deque;
using std::vector;
using std::pair;

void FlipColor(int x, int y, vector<deque<bool>>* image_ptr) {
	const int maxX = image_ptr->size() - 1;
	const int maxY = (*image_ptr)[0].size() - 1;
	deque<pair<int, int>> work;
	bool colour = !(*image_ptr)[x][y];
	work.push_back({ x, y });
	while (work.size()) {
		auto p = work.front();
		work.pop_front();

		int xx = p.first;
		int yy = p.second;

		deque<bool> &row = (*image_ptr)[xx];
		if (row[yy] ^ colour) {
			row[yy] = colour;

			if (yy > 0 && row[yy - 1] ^ colour) work.push_back({ xx, yy - 1 });
			if (yy < maxY && row[yy + 1] ^ colour) work.push_back({ xx, yy + 1 });
			if (xx > 0) work.push_back({ xx - 1, yy });
			if (xx < maxX) work.push_back({ xx + 1, yy });
		}
	}
	return;
}
vector<vector<int>> FlipColorWrapper(TimedExecutor& executor, int x, int y,
	vector<vector<int>> image) {
	vector<deque<bool>> b;
	b.reserve(image.size());
	for (const vector<int>& row : image) {
		deque<bool> tmp;
		tmp.resize(row.size());
		for (int i = 0; i < row.size(); ++i) {
			tmp[i] = static_cast<bool>(row[i]);
		}
		b.push_back(tmp);
	}

	executor.Run([&] { FlipColor(x, y, &b); });

	image.resize(b.size());

	for (int i = 0; i < image.size(); ++i) {
		image[i].resize(b.size());
		for (int j = 0; j < image[i].size(); ++j) {
			image[i][j] = b[i][j];
		}
	}
	return image;
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "executor", "x", "y", "image" };
	return GenericTestMain(args, "matrix_connected_regions.cc", "painting.tsv",
		&FlipColorWrapper, DefaultComparator{}, param_names);
}
