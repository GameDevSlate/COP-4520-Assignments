#include "Assignment_3.h"

#include <iostream>
#include <random>
#include <ranges>


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

	std::cout << "Thanking all the guests\n";

	for (auto& worker : workers)
		worker.wait();

	std::cout << "\nAll guests are " << (gifts.Empty() ? "not " : "") << "thanked\n";

	// Now, get the time when we finished the program.
	auto end = std::chrono::high_resolution_clock::now();

	auto total_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

	std::printf("It took %lldms for the operations to complete.\n\n", total_time);
	

	// ***** PART 2 *****


	// Create the threads
	std::array<std::future<void>, 8> sensors;

	// Add a new vector to each thread id, and allocate their function
	for(int i = 0; i < sensors.size(); i++) {

		sensors_data[i].reserve(60);
		sensors[i] = std::async(std::launch::async, &Assignment_3::ReadTemperature, this, i);
	}

	// Set main thread to sleep for simulated time (60 seconds)
	std::cout << "Recording sensor data for 1 simulated hour (1 minute).\n";

	
	std::this_thread::sleep_for(std::chrono::seconds(reading_amount));

	stop_reading = true;

	for (auto& sensor : sensors)
		sensor.wait();

	PrintReport();
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

void Assignment_3::ReadTemperature(const int sensor_id)
{
	// Create the Random Number Generator to get random temperatures
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution number_range(-100, 70);

	// Keep reading data until it is globally known to stop
	while(!stop_reading) {

		// Get random temp
		int degrees = number_range(gen);

		// Get the timestamp
		auto time_point = std::chrono::steady_clock::now();

		sensors_data[sensor_id].emplace_back(time_point, degrees);

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void Assignment_3::PrintReport()
{
	// Make vector that holds all temperatures from all sensors
	std::vector<std::pair<std::chrono::steady_clock::time_point, int>> all_temps;

	all_temps.reserve(60 * sensors_data.size());

	for (auto sensor : sensors_data | std::views::values)
		all_temps.insert(all_temps.end(), sensor.begin(), sensor.end());

	// Get the top 5 highest and lowest temps
	std::ranges::sort(all_temps,
		[](const std::pair<std::chrono::steady_clock::time_point, int>& data1, decltype(data1) data2) { return data1.second < data2.second; });

	std::cout << "The top highest temperatures recorded were: ";

	for (int i = all_temps.size() - 1; i > all_temps.size() - 6; i--)
		std::printf("%d%s", all_temps[i].second, i > all_temps.size() - 5 ? ", " : "\n");
	
	std::cout << "The top lowest temperatures recorded were: ";

	for (int i = 0; i < 5; i++)
		std::printf("%d%s", all_temps[i].second, i < 4 ? ", " : "\n");
}
