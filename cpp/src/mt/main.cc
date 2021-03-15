#include <functional>
#include <iostream>
#include <thread>
#include <vector>

#define MIN_THREADS 2

void hello() {
	std::cout << "Hello " << std::this_thread::get_id() << std::endl;
}

int main() {
	unsigned int ht = std::thread::hardware_concurrency();
	unsigned int nt = (ht == 0) ? MIN_THREADS : ht;

	std::vector<std::thread> ts(nt - 1);

	for (unsigned i = 0; i < nt - 1; ++i) {
		ts[i] = std::thread(hello);
	}

	std::for_each(ts.begin(), ts.end(), std::mem_fn(&std::thread::join));

	return 0;
}
