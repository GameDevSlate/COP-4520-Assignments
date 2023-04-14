# Assignment 3 Report

## Problem 1
### Implementation
 Since C++ does not have a lot of built-in data types, specially for concurrency, I wrote an implementation for a *Concurrent Singly Linked List* that uses atomic operations. This was done by creating a `Node` struct where its `next` pointer was an atomic smart pointer, that way, I also do not have to worry about memory management.
 
Setting the data for the atomic smart pointers was as simple as using a member function for C++ atomics called `compare_exchange_weak`, which is very similar to the theoretical *Compare and Set* function that is taught in this course.

### Results and Performance
The `std::async` function allowed me easily to test execution time between single-threaded and multi-threaded runtime. These are example results of one execution, however, the speed increase was estimated from looking at *10 different execution times*, and averaging them out:

#### Single-Threaded
```
All guests are thanked
It took 3610ms for the operations to complete.
```
#### Multi-Threaded
```
All guests are thanked
It took 828ms for the operations to complete.
```
---
The average of all the execution times showed that the speed in-execution increased by *~4.32 times* when the application was converted to multi-threaded.

## Problem 2
### Implementation
For reading temperatures for each thread, I simply made a function where it would create a random number generator, it would then loop infinitely until a global boolean was set to false. Inside of the loop a new number within the range of allowed temperatures would be generated and added to a Hashset where the key is the thread id, while the value is a vector of the thread's stored temperatures. Lastly the thread would sleep for 1 second, this is because the simulated time is one minute, while data has to be recorded every second.

The main thread allocates all other threads for the sensors and puts itself to sleep for 60 seconds, when it wakes up, it sets the global boolean to false so that all sensor threads exit out of their loops.

### Performance
This cannot really be measured since the program will always run for one minute, single or multi threaded. However the implementation proved to consitently collect 60 temperature values for each thread.
