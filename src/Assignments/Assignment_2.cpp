#include "Assignment_2.h"

#include <chrono>
#include <queue>
#include <random>

Assignment_2::Assignment_2()
{
	// ---------- PART ONE ----------

	// Assign the cupcake as a lock. Starting the lock
	// as unlocked to clarify that there is already a cupcake
	m_cupCake = std::unique_lock(m_eatCupcake, std::defer_lock);

	std::cout << "How many guests would you like the Minotaur to invite?\n";

	int temp_amount;

	std::cin >> temp_amount;

	// Simple input validation
	if (temp_amount < 1) temp_amount = 1;

	m_guestCount = temp_amount;

	// Set the vector used so that each guest knows if they have
	// eaten a cupcake before or not
	m_guestsTask = std::vector(m_guestCount, false);

	// The leader will always enter first,
	// but they do not eat the cupcake
	m_guestsTask[0] = true;

	// Star tracking time
	auto start_time = std::chrono::high_resolution_clock::now();

	// Create a vector that holds all the guests as std::future's
	std::vector<std::future<void>> guests(m_guestCount);

	// The minotaur which picks guest at random will be a random
	// number generator
	std::random_device rd;
	std::mt19937 minotaur(rd());
	std::uniform_int_distribution distribution(0, m_guestCount - 1);

	while(m_leaderCount < m_guestCount) {

		// Get a random index from the vector
		auto picked_guest = distribution(minotaur);

		guests[picked_guest] = std::async(std::launch::async, &Assignment_2::EnterLabyrinth, this, picked_guest);
		guests[picked_guest].wait();
	}

	auto end_time = std::chrono::high_resolution_clock::now();

	auto total_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

	std::printf("Total time for all guests to have the labyrinth at least once: %lld microseconds\n\n", total_time);

	// ---------- PART TWO ----------

	// reset the current task of all the prisoners
	for (auto&& task : m_guestsTask)
		task = false;

	// Have all the prisoners enter a queue, this is Method #3 from the document
	std::queue<std::future<void>> guests_queue;

	start_time = std::chrono::high_resolution_clock::now();

	// Keep adding the order of how guests have entered the queue.
	// Because the number of the guest is pulled randomly to simulate a guest
	// re-entering the queue, this also implies some window of time where no,
	// new guest was entering the queue.
	while (!std::ranges::all_of(m_guestsTask, [](const bool task) {return task == true; })) {
		
		auto entering_guest = distribution(minotaur);

		guests_queue.emplace(std::async(std::launch::async, &Assignment_2::CheckoutVase, this, entering_guest));
	}

	// de-queue the entire queue
	while (!guests_queue.empty())
		guests_queue.pop();

	end_time = std::chrono::high_resolution_clock::now();

	total_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

	std::printf("Total time it took for all guests to checkout the vase at least once: %lld microseconds\n", total_time);

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

#ifdef _DEBUG
		std::cout << "Leader has counted: " << m_leaderCount << '\n';
#endif

		m_cupCake.unlock();
		return;
	}

	// If it is anyone else than the leader, there is a cupcake,
	// and they have not eaten a cupcake before, they eat it,
	// but they will not order a new cupcake
	if (thread_num != 0 && !m_cupCake.owns_lock() && !m_guestsTask[thread_num]) {
		m_cupCake.lock();
		m_guestsTask[thread_num] = true;

#ifdef _DEBUG
		std::printf("Guest %d has eaten cupcake.\n", thread_num + 1);
#endif


		// Minor check to see if the leader's count has already reached the guest count
		if (m_leaderCount == m_guestCount)
			m_cupCake.unlock();
	}
}

void Assignment_2::CheckoutVase(const int thread_num)
{
	// This if-statement servers as when a guest that
	// has already entered once has entered the room again.
	if (m_guestsTask[thread_num]) return;

	// The lock here represents as if a guest has entered the
	// room to check out the vase for their first time.
	std::lock_guard next_guest(m_enterRoom);

	m_guestsTask[thread_num] = true;

#ifdef _DEBUG
	std::printf("Guest number %d has entered checkout vase and announced next person to come.\n", thread_num + 1);
#endif
}

