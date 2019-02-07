#include <vector>

#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/test_failure.h"
class Queue {
	std::vector<int> storage;
	int s = 0, size = 0;

public:
	Queue(size_t capacity) : storage(capacity) {}

	void Enqueue(int x) {
		const int capacity = storage.size();
		if (size == capacity) {
			auto old_storage = storage;
			storage = std::vector<int>(capacity * 2);
			for (int i = 0; i < size; i++) {
				storage[i] = old_storage[(s + i) % capacity];
			}
			s = 0;
		}
		storage[(s + size++) % /* new */ storage.size()] = x;
	}
	int Dequeue() {
		if (size-- == 0) return -1;
		int res = storage[s];
		s = (s + 1) % storage.size();
		return res;
	}
	int Size() const {
		return size;
	}
};
struct QueueOp {
	enum { kConstruct, kDequeue, kEnqueue, kSize } op;
	int argument;

	QueueOp(const std::string& op_string, int arg) : argument(arg) {
		if (op_string == "Queue") {
			op = kConstruct;
		}
		else if (op_string == "dequeue") {
			op = kDequeue;
		}
		else if (op_string == "enqueue") {
			op = kEnqueue;
		}
		else if (op_string == "size") {
			op = kSize;
		}
		else {
			throw std::runtime_error("Unsupported queue operation: " + op_string);
		}
	}

	void execute(Queue& q) const {
		switch (op) {
		case kConstruct:
			// Hack to bypass deleted assign operator
			q.~Queue();
			new (&q) Queue(argument);
			break;
		case kDequeue: {
			int result = q.Dequeue();
			if (result != argument) {
				throw TestFailure("Dequeue: expected " + std::to_string(argument) +
					", got " + std::to_string(result));
			}
		} break;
		case kEnqueue:
			q.Enqueue(argument);
			break;
		case kSize: {
			int s = q.Size();
			if (s != argument) {
				throw TestFailure("Size: expected " + std::to_string(argument) +
					", got " + std::to_string(s));
			}
		} break;
		}
	}
};

template <>
struct SerializationTraits<QueueOp> : UserSerTraits<QueueOp, std::string, int> {
};

void QueueTester(const std::vector<QueueOp>& ops) {
	Queue q(0);
	for (auto& op : ops) {
		op.execute(q);
	}
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "ops" };
	return GenericTestMain(args, "circular_queue.cc", "circular_queue.tsv",
		&QueueTester, DefaultComparator{}, param_names);
}
