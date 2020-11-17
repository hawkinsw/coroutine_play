#include <iostream>
#include <experimental/coroutine>

using namespace std;
using namespace std::experimental;

struct random_generator {
	struct promise_type {
		random_generator get_return_object()
		{
			return random_generator(coroutine_handle<promise_type>::from_promise(*this));
		}

		auto initial_suspend() { return suspend_never{}; }
		auto final_suspend() { return suspend_never{}; }
		void return_void() { }
		void unhandled_exception() {}
	};

	coroutine_handle<promise_type> _coroutine = nullptr;
	explicit random_generator(coroutine_handle<promise_type> coroutine) : _coroutine(coroutine) {
	}

	~random_generator() {
		if (_coroutine) {
			_coroutine.destroy(); 
		}
	}

	random_generator() = default;
	random_generator(random_generator const &) = delete; // cannot copy
	random_generator(random_generator &&other) : _coroutine(other._coroutine){
		other._coroutine = nullptr;
	}

	random_generator &operator=(random_generator &&other) {
		if (&other != this) {
			_coroutine = other._coroutine;
			other._coroutine = nullptr;
		}
		return *this;
	}

	void resume() {
		if (_coroutine) {
			_coroutine.resume();
		}
	}

};

int main() {
	cout << "Building a random generator." << endl;
	random_generator rand_gen = counter();
	rand_gen.resume();
	rand_gen.resume();
	rand_gen.resume();
	cout << "Done." << endl;
}
