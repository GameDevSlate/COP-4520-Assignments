#include "Assignment_3.h"

#include <algorithm>
#include <bitset>
#include <random>

// Make a Node struct for the guests
struct Node
{
	int guestNumber;
	std::atomic<std::shared_ptr<Node>> next;

	explicit Node(const int val) : guestNumber(val), next(nullptr){}
};

// Make a class for an ordered concurrent Linked-list
class ConcurrentLinkedList
{
public:
	std::atomic<std::shared_ptr<Node>> head;

	ConcurrentLinkedList() : head(nullptr){}

	void Insert(int guest_number)
	{
		auto new_node = std::make_shared<Node>(guest_number);
		auto curr_head = head.load();

		// This loop can safely be while (true) since, all cases
		// inside will return
		while (true) {

			// If the head is empty, insert as new head
			if(!curr_head && head.compare_exchange_strong(curr_head, new_node)) {
				return;
			}

			// Insert before head if the new value is smaller than it
			if (guest_number < curr_head->guestNumber) {
				new_node->next = curr_head;

				if (head.compare_exchange_strong(curr_head, new_node))
					return;
			}

			// Go through list and insert at correct position
			else {

				auto pre_node = curr_head;
				auto curr_node = pre_node->next.load();

				// Check that the current node is not null, and that its value
				// is less than the given guest number
				while(curr_node && curr_node->guestNumber < guest_number) {
					pre_node = curr_node;
					curr_node = pre_node->next.load();
				}

				new_node->next = curr_node;

				if(pre_node->next.compare_exchange_strong(curr_node, new_node))
					return;
			}

			curr_head = head.load();
		}
	}

	// Thread-safe removal
	bool Remove(const int guest_number)
	{
		auto curr_head = head.load();

		while(curr_head && curr_head->guestNumber <= guest_number) {

			if (curr_head->guestNumber == guest_number && head.compare_exchange_strong(curr_head, curr_head->next))
				return true;

			curr_head = curr_head->next.load();
		}

		return false;
	}
};

Assignment_3::Assignment_3()
{
	// Pre-allocate all 500,000 guests into an array
	// in random order

	int guests[500000];

	for (int i = 0; i < 500000; i++)
		guests[i] = i;

	std::random_device rd;
	std::mt19937 gen(rd());

	// Shuffle so that all guests are placed randomly
	std::shuffle(guests, guests + 500000, gen);

	// Make a bit set that will be checked later
	std::bitset<500000>guests_thanked;

}
