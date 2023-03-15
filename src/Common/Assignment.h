#pragma once
// Adding iostream, since I might have to read user input
#include <iostream>

// Adding all of the multi-threading libraries, since
// I don't know what might and might not be used.
#include <atomic>
#include <condition_variable>
#include <future>
#include <mutex>
#include <thread>

// Smart pointers so I don't have to worry too much about
// memory management
#include <memory>

template < typename T >
using ref = std::unique_ptr<T>;

template < typename T >
using shared_ref = std::shared_ptr<T>;

// The way assignments will work is by having a terminal UI
// that holds Assignment types based, once the user selects
// what assignment they want to use (either through a
// command-line argument, or with the terminal GUI,
// it will show that assignment. Because of this, I will have
// a function that calls another function that creates
// an assignment type. To do this, I need the 'functional'
// library, in order to have function objects.
#include <functional>

// Need to store the assignment environments.
#include <vector>

#include <concepts>

class Assignment
{
public:
	Assignment() = default;

	virtual ~Assignment() = default;

	virtual void AssignmentText(int choice = -1) {}
};

template < typename T >
concept assignment_child = std::derived_from<T, Assignment>;

class AssignmentMenu final : public Assignment
{
public:
	explicit
		AssignmentMenu(Assignment*& current_assignment_pointer) : m_currentAssignment(current_assignment_pointer) {}

	void AssignmentText(int choice = -1) override
	{
		if (choice == -1 || (choice < 0 || static_cast<size_t>(choice) >= m_assignments.size())) {
			// Cool little menu logo if command-line arguments are not used.
			std::cout << R"(
    ___              _                                  __      
   /   |  __________(_)___ _____  ____ ___  ___  ____  / /______
  / /| | / ___/ ___/ / __ `/ __ \/ __ `__ \/ _ \/ __ \/ __/ ___/
 / ___ |(__  |__  ) / /_/ / / / / / / / / /  __/ / / / /_(__  ) 
/_/  |_/____/____/_/\__, /_/ /_/_/ /_/ /_/\___/_/ /_/\__/____/  
                   /____/                                       
)";

			for (size_t i = 0; i < m_assignments.size(); i++)
				std::printf("%llu. %s.\n", i + 1, m_assignments[i].first.c_str());

			std::cout << "Type the number of the assignment, then press ENTER.\n";

			std::cin >> choice;
		}

		std::cout << "\033[2J\033[1;1H";

		m_currentAssignment = m_assignments[--choice].second();
	}

	/**
	 * \brief Creates and adds a new assignment to the list of assignments.
	 * \param name of the assignment.
	 */
	template < assignment_child T >
	void RegisterAssignment(const std::string& name)
	{
		m_assignments.emplace_back(name, [] { return new T(); });
	}

private:
	// Reference to a pointer because the instances will be dynamically created.
	Assignment*& m_currentAssignment;

	// What holds all of the assignments.
	std::vector<std::pair<std::string, std::function<Assignment* ()>>> m_assignments;
};
