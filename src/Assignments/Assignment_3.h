#pragma once

#include <array>
#include <bitset>

#include "../Common/Assignment.h"

class Assignment_3 final : public Assignment
{
public:

	Assignment_3();

private:

	// Make a Node struct for the guests
	struct Node
	{
		int guestNumber;
		std::atomic<std::shared_ptr<Node>> next;

		explicit Node(const int val) : guestNumber(val), next(nullptr) {}
	};

	// Make a class for an ordered concurrent Linked-list
	class ConcurrentLinkedList
	{
	public:
		std::atomic<std::shared_ptr<Node>> head;

		ConcurrentLinkedList() : head(nullptr) {}

		void Insert(int guest_number)
		{
			// Create a new guest node
			auto insert_guest = std::make_shared<Node>(guest_number);
			auto current_guest = head.load();
			std::shared_ptr<Node> previous_guest = nullptr;

			// Traverse the list until we reach the location to insert
			while(current_guest != nullptr && current_guest->guestNumber < guest_number) {
				previous_guest = current_guest;
				current_guest = current_guest->next.load();
			}

			do insert_guest->next = current_guest;
			// compare and swap
			while (previous_guest != nullptr && !previous_guest->next.compare_exchange_weak(current_guest, insert_guest));

			// In case the list was empty
			if (previous_guest == nullptr)
				head.compare_exchange_weak(current_guest, insert_guest);
		}

		// Thread-safe removal
		bool Remove(const int guest_number)
		{
			std::shared_ptr<Node> current_guest = head.load();
			std::shared_ptr<Node> previous_guest = nullptr;

			// Traverse until end of list reached
			while (current_guest != nullptr) {

				if (current_guest->guestNumber == guest_number) {
					std::shared_ptr<Node> next_guest = current_guest->next.load();

					// If the guest being deleted is head
					if (previous_guest == nullptr) {
						if (head.compare_exchange_weak(current_guest, next_guest)) 
							return true;
					}
					else {
						if (previous_guest->next.compare_exchange_weak(current_guest, next_guest)) 
							return true;
					}
				}
				previous_guest = current_guest;
				current_guest = current_guest->next.load();
			}

			// If guest was not found
			return false;
		}

		bool Empty() const { return head.load() == nullptr; }
	};

	void AddAndThank(int worker_num);

	// Part 1
	std::unique_ptr<std::array<int, 500000>> guests;

	ConcurrentLinkedList gifts;

	// Condition variable that tells all the workers to remove
	// a gift from the list when the last worker has inserted
	std::condition_variable ready_to_thank;

	// Condition variable that tells all the workers to insert
	// again
	std::condition_variable ready_to_insert;
};
