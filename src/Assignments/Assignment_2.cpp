#include "Assignment_2.h"

#include <algorithm>
#include <chrono>
#include <random>

Assignment_2::Assignment_2()
{
	// Assign the cupcake as a lock. Starting the lock
	// as unlocked to clarify that there is already a cupcake
	m_cupCake = std::unique_lock(m_eatCupcake, std::defer_lock);

	// Get the maximum number of threads the hardware supports
	auto thread_max = std::thread::hardware_concurrency();

	std::cout << "How many guests would you like the Minotaur to invite? (Maximum is: " << thread_max << ")\n";

	int temp_amount;

	std::cin >> temp_amount;

	// Simple input validation
	if (temp_amount < 1) temp_amount = 1;
	else if (temp_amount > thread_max) temp_amount = thread_max;

	m_guestCount = temp_amount;

	// Set the vector used so that each guest knows if they have
	// eaten a cupcake before or not
	m_prisonerCupcakes = std::vector(m_guestCount, false);

	// The leader will always enter first, and so,
	// but they do not eat the cupcake
	m_prisonerCupcakes[0] = true;

	// Star tracking time
	auto start_time = std::chrono::high_resolution_clock::now();

	// Create a vector that holds all the guests as std::future's
	std::vector<std::future<void>> guests(m_guestCount);

	// The minotaur which picks guest at random will be a random
	// number generator
	std::random_device rd;
	std::mt19937 minotaur(rd());
	std::uniform_int_distribution distribution(0, m_guestCount - 1);

	while(true) {

		// Get a random index from the vector
		auto picked_guest = distribution(minotaur);

		guests[picked_guest] = std::async(std::launch::async, &Assignment_2::EnterLabyrinth, this, picked_guest);
		guests[picked_guest].wait();

		if (m_leaderCount == m_guestCount - 1)
			break;
	}

	auto end_time = std::chrono::high_resolution_clock::now();

	auto total_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

	std::printf("Total time for all guests to have the labyrinth at least once: %lld microseconds\n", total_time);
	
}

Assignment_2::~Assignment_2()
{

	m_cupCake.release();
}

void Assignment_2::EnterLabyrinth(const int thread_num)
{

	// For greater clarity:
	// cupcake does not own lock	= there is a cupcake
	// cupcake owns lock			= there is no cupcake

	// If the leader enters the room and there is no cupcake,
	// they will ask for one, not eat it, and increase their count
	if (thread_num == 0 && m_cupCake.owns_lock()) {
		++m_leaderCount;
		m_cupCake.unlock();
		return;
	}

	// If it is anyone else than the leader, there is a cupcake,
	// and they have not eaten a cupcake before, they eat it,
	// but they will not order a new cupcake
	if (thread_num != 0 && !m_cupCake.owns_lock() && !m_prisonerCupcakes[thread_num]) {
		m_cupCake.lock();
		m_prisonerCupcakes[thread_num] = true;

		// Minor check to see if the leader's count has already reached the guest count
		if (m_leaderCount == m_guestCount - 1)
			m_cupCake.unlock();
	}


}


