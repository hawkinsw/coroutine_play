#include <iostream>
#include <experimental/coroutine>

using namespace std;
using namespace std::experimental;

struct resumable_thing {
	struct promise_type {
		resumable_thing get_return_object()
		{
			return resumable_thing(coroutine_handle<promise_type>::from_promise(*this));
		}

		auto initial_suspend() { return suspend_never{}; }
		auto final_suspend() { return suspend_never{}; }
		void return_void() { }
		void unhandled_exception() {}
	};

	coroutine_handle<promise_type> _coroutine = nullptr;
	explicit resumable_thing(coroutine_handle<promise_type> coroutine) : _coroutine(coroutine) {
	}

	~resumable_thing() {
		if (_coroutine) {
			_coroutine.destroy(); 
		}
	}

	resumable_thing() = default;
	resumable_thing(resumable_thing const &) = delete; // cannot copy
	resumable_thing(resumable_thing &&other) : _coroutine(other._coroutine){
		other._coroutine = nullptr;
	}

	resumable_thing &operator=(resumable_thing &&other) {
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

resumable_thing counter() {
	cout << "counter called." << endl;
	for (unsigned int i = 1; ; i++) {
		co_await suspend_always{};
		cout << "counter: resumed (#" << i << ")" << endl;
	}
}

int main() {
	cout << "Building a counter." << endl;
	resumable_thing the_resumable_counter = counter();
	the_resumable_counter.resume();
	the_resumable_counter.resume();
	the_resumable_counter.resume();
	cout << "Done." << endl;
}
