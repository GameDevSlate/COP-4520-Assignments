#include "Assignment_3.h"

#include <algorithm>
#include <random>

Assignment_3::Assignment_3()
{
	// ***** PART 1 *****
	// Pre-allocate all 500,000 guests into an array
	// in random order
	guests = std::make_unique<std::array<int, 500000>>();

	for (int i = 0; i < 500000; i++)
		guests->at(i) = i;

	std::random_device rd;
	std::mt19937 gen(rd());

	// Shuffle so that all guests are placed randomly
	std::ranges::shuffle(*guests, gen);

	// Create the worker threads
	std::array<std::future<void>, 4> workers;

	for(int i = 0; i < 4; i++) {

		// Assign the threads their alternating task of adding a guest
		// and removing it from the list until the guests array has been
		// completely iterated
		workers[i] = std::async(std::launch::async, &Assignment_3::AddAndThank, this, i);
	}

	// To know the total elapsed time, get the current time when the program started.
	auto start = std::chrono::high_resolution_clock::now();

	for (auto& worker : workers)
		worker.wait();

	std::cout << "\nAll guests are " << (gifts.Empty() ? "not" : "") << "thanked\n";

	// Now, get the time when we finished the program.
	auto end = std::chrono::high_resolution_clock::now();

	auto total_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

	std::printf("It took %lldms for the operations to complete.\n", total_time);

	// ***** PART 2 *****

}

void Assignment_3::AddAndThank(const int worker_num)
{
	for (int j = worker_num; j < 500000 - worker_num; j += 4) {

#if _DEBUG
		std::printf("Adding guest %d to the list\n", guests->at(j));
#endif

		// Add guest to list
		gifts.Insert(guests->at(j));

		// Send "Thank you" letter (remove) to guest
		gifts.Remove(guests->at(j));

#if _DEBUG
		std::printf("Thanked guest %d\n", guests->at(j));
#endif
	}
}