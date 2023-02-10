// COP-4520-Assignments-Converted.cpp : Defines the entry point for the application.
//

#include "Assignments/Assignment_1.h"
#include "Assignments/Assignment_2.h"
#include "Common/Assignment.h"


const int MIN_ASSIGNMENT = 1;

const int MAX_ASSIGNMENT = 1;

int main(const int argc, char* argv[])
{
	// Make the assignments menu, and the current assignment that the user will enter
	Assignment* current_assignment = nullptr;
	auto menu = new AssignmentMenu(current_assignment);

	current_assignment = menu;

	menu->RegisterAssignment<Assignment_1>("Prime statistics");
	menu->RegisterAssignment<Assignment_2>("Minotaur\'s Party");

	// Check if command-line arguments are being passed
	if (argc == 2) {
		if (const int command = std::strtol(argv[1], nullptr, 10);
			command >= MIN_ASSIGNMENT && command <= MAX_ASSIGNMENT)
			current_assignment->AssignmentText(command);
	} else
		current_assignment->AssignmentText();

	delete current_assignment;

	if (current_assignment != menu)
		delete menu;

	std::cout << "Press the ENTER key to exit.\n";
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.get();
}
