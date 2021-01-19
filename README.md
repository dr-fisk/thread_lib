# thread_lib
Created a thread library which is capable of creating new threads, scheduling the threads, and then the threads are collected and cleaned up when all threads are finished scheduling. The queue is implemented using a singly linked list.
The library uses a counting semaphore to lock resources. You can test the library by running the executables in the apps folder. Simply cd into apps
and issue the command make. This will create the executables to test the code.

# TODO: 
Add preemption to the scheduler. Currently, there is no preemption, but the preemption intialization code is completed.
