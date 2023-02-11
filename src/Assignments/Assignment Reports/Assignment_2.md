# Assignment 2 Report

## Common Approaches for Both Problems

Both problems required some amount of repetition:

1. It is stated in problem one that, "...the Minotaur may pick the same guests multiple times and ask them to enter the labyrinth."

2. For the approach that I took—which will be talked about in its own section later—"Guests were allowed to queue multiple times."

Because of this, I created a Pseudo-Random Number Generator in C++ that uses the `std::mersenne_twister_engine` that would later be used inside of a loop which would return an integer in the range of [1,*N*], where *N* is whatever number of guests the user input.

### Common Implementations for Both Problems

Both problems required so that guests would have a memory of recalling whether they have done a task before (like humans), so I created a C++ `vector` one type `bool` so that a guests knows whether they have done a task before (such as entering a labyrinth, or checking out a vase), this was called `m_guestsTasks`.

## Problem 1

Here are the steps that I took to solve problem 1:

* The will have a manager who will be in charge of counting.
* When a guest, who is other than the manager, completes that labyrinth and there is a cupcake, they will eat it *if* they have not eaten a cupcake before.
* If there is a cupcake when the manager completes the labyrinth, they will leave it untouched. If there is no cupcake, the manager will order a new one, *not* eat it, and add one to his count.
* When the manager’s count gets to *N* - 1, they will state that all *N* guests have entered the labyrinth.

### Implementation

Since there is no particular order of the guests (re-)entering the labyrinth, I first created a `vector` that would hold `std::future<void>`, this vector would be the actual guests, since each of them has to be an individual thread; as mentioned in Assignment 1, futures are handled through the call of the `std::async` function, which can be explicitly told to run asynchronously. I treated the concept of the cupcake to be a C++ `std::unique_lock`, which unlike `std::lock_guard`, it can exist outside the scope of a function; once the cupcake was eaten, it would be locked, which allowed for the safe editing of `m_guestsTasks` to either turn convert the boolean value that corresponds to the number of an individual guest into `true`, or do whatever required logic; in this locked state, it is also safe for the leader to update their count, when they complete the labyrinth. All of the logic that that was mentioned is in the function `EnterLabyrinth`.

---

## Problem 2

For this, problem, I think guests should choose option three:

> ...allow the guests to line in a queue. Every guest exiting the 
room was responsible to notify the guest standing in front of the queue that the 
showroom is available. Guests were allowed to queue multiple times.

### Advantage

In a pragmatic scenario, the queue mitages the time spent when there are no guests entering the vase room and has following logic:

* If there is no one checking out the vase, the door to the vase room is left open. That is the way to signal from one guest (or none) to another that they may enter the room.
* When a guest enters the vase room, they close and lock the door.
* If the vase room's door is locked, any guests that want to enter it have to enqueue in the order at which they have arrived.
* When a guest leaves the vase room, they unlock the door and leave it open, and whichever other guest is at the front of the queue, de-queues it to now enter the room and lock the door.

This logic is done until all guests have checkout the vase at least once. It also makes for an organized way for the guests to enter the room, rather than for them to fight as to who should enter the room first, which can take up time.

### Disadvantage

The only small disadvantage is simply that there might still be some range of time where there might not be anyone checking out the vase, which theoretically means that all the guests are doing something else.

### Implementation

First, I re-set the `m_guestsTask` vector to all be `false`, afterwards, I create a queue of `std::future<void>` to represent the guests themselves as threads by calling the `std::async` function again with the explicit parameter to run asynchronously. I repurposed the PRNG from problem one again to simulate the random order of guests enqueing, which also allowed for guests to enqueue again. However, I created a function called `CheckoutVase` in which internally, it would have an `std::lock_guard` that would be the notifier for guests de-queue and enter the room, so if the lock was owned, no guest can enter the room, once the current guest in the room leaves, the lock would be destroyed, which allows for the next guest to enter. This would all be done inside of a loop that would run until all the values of `m_guestsTask` would be true, since **it is not specified in this problem** that guests cannot communicate with one another.