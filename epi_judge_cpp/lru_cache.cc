#include <vector>
#include <unordered_map>
#include "test_framework/generic_test.h"
#include "test_framework/serialization_traits.h"
#include "test_framework/test_failure.h"

using std::unordered_map;

struct lru_entry {
	int isbn;
	int price;
	lru_entry *next;
	lru_entry *prev;
};


class LruCache {
	lru_entry last{ -2, -2, nullptr, nullptr };
	lru_entry first{ -1, -1, &last, nullptr };
	unordered_map<int, lru_entry*> index;
	size_t capacity;

	void delete_entry(const lru_entry &entry) {
		entry.prev->next = entry.next;
		entry.next->prev = entry.prev;
	}

	void add_entry(lru_entry *entry) {
		first.next->prev = entry;
		first.next = entry;
	}

	void refresh_entry(lru_entry *entry) {
		delete_entry(*entry);
		entry->prev = &first;
		entry->next = first.next;
		add_entry(entry);
	}

public:
	LruCache(size_t capacity) {
		this->capacity = capacity;
		last.prev = &first;
	}

	int Lookup(int isbn) {
		if (index.count(isbn)) {
			auto cached = index[isbn];
			refresh_entry(cached);
			return (cached)->price;
		}
		return -1;
	}

	void Insert(int isbn, int price) {
		if (index.count(isbn)) {
			auto cached = index[isbn];
			//cached->price = price;
			refresh_entry(cached);
		}
		else {
			if (index.size() == capacity) {
				lru_entry *removed = last.prev;
				delete_entry(*removed);
				index.erase(removed->isbn);
				delete(removed);
			}
			lru_entry *entry = new lru_entry{ isbn, price, first.next, &first };
			add_entry(entry);
			index[isbn] = entry;
		}
	}
	bool Erase(int isbn) {
		if (index.count(isbn)) {
			auto cached = index[isbn];
			delete_entry(*cached);
			index.erase(isbn);
			return true;
		}
		return false;
	}
};
struct Op {
	std::string code;
	int arg1;
	int arg2;
};

template <>
struct SerializationTraits<Op> : UserSerTraits<Op, std::string, int, int> {};

void RunTest(const std::vector<Op>& commands) {
	if (commands.empty() || commands[0].code != "LruCache") {
		throw std::runtime_error("Expected LruCache as first command");
	}
	LruCache cache(commands[0].arg1);

	for (int i = 1; i < commands.size(); i++) {
		auto& cmd = commands[i];
		if (cmd.code == "lookup") {
			int result = cache.Lookup(cmd.arg1);
			if (result != cmd.arg2) {
				throw TestFailure("Lookup: expected " + std::to_string(cmd.arg2) +
					", got " + std::to_string(result));
			}
		}
		else if (cmd.code == "insert") {
			cache.Insert(cmd.arg1, cmd.arg2);
		}
		else if (cmd.code == "erase") {
			bool result = cache.Erase(cmd.arg1);
			if (result != cmd.arg2) {
				throw TestFailure("Erase: expected " + std::to_string(cmd.arg2) +
					", got " + std::to_string(result));
			}
		}
		else {
			throw std::runtime_error("Unexpected command " + cmd.code);
		}
	}
}

int main(int argc, char* argv[]) {
	std::vector<std::string> args{ argv + 1, argv + argc };
	std::vector<std::string> param_names{ "commands" };
	return GenericTestMain(args, "lru_cache.cc", "lru_cache.tsv", &RunTest,
		DefaultComparator{}, param_names);
}
