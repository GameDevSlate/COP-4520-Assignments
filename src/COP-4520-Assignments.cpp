// COP-4520-Assignments-Converted.cpp : Defines the entry point for the application.
//

#include "Assignments/Assignment_1.h"
#include "Assignments/Assignment_2.h"
#include "Assignments/Assignment_3.h"
#include "Common/Assignment.h"


int main(const int argc, char* argv[])
{
	// Make the assignments menu, and the current assignment that the user will enter
	Assignment* current_assignment = nullptr;
	auto menu = new AssignmentMenu(current_assignment);

	current_assignment = menu;

	menu->RegisterAssignment<Assignment_1>("Prime statistics");
	menu->RegisterAssignment<Assignment_2>("Minotaur\'s Party");
	menu->RegisterAssignment<Assignment_3>("Minotaur Post Party and Sensors Statistics");

	// Check if command-line arguments are being passed
	const int command = argc == 2 ? std::strtol(argv[1], nullptr, 10) : -1;

	current_assignment->AssignmentText(command);

	delete current_assignment;

	if (current_assignment != menu)
		delete menu;

	std::cout << "Press the ENTER key to exit.\n";
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.get();
}
