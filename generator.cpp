#include <iostream>
#include <random>
#include <algorithm>
#include <numeric>
#include <vector>
#include <experimental/coroutine>
#include "mcnellis_generator.h"

generator<int> next_random(int limit=0) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 10);
	for (auto generated = 0; generated<limit; generated++) {
		co_yield dis(gen);
		std::cout << "Yielded a value." << '\n';
	}
}

struct random_generator {
	private:
		std::random_device rd;
		std::mt19937 gen;
		std::uniform_int_distribution<> dis;
	public:
		random_generator(const random_generator &other) {
			gen = other.gen;
			dis = other.dis;
		}

		random_generator() {
			gen = std::mt19937(rd());
			dis = std::uniform_int_distribution(0,10);
		}

		random_generator& operator ++() {
			return *this;
		}

		operator int() {
			std::cout << "Another random." << '\n';
			return dis(gen);
		}
};

int main() {
	// Fill it up with some values
	std::vector<int> range(100);
	random_generator random_generated;
	for (int i =0; i<range.size(); i++) {
		range[i] = random_generated;
	}

	auto nr = next_random(100);
	for (auto i : nr) {
		std::cout << "random: " << i << '\n';
	}
}
