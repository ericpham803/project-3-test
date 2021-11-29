Group: Eric Pham, Gregory Marinakis

buffer:

producer: The producer thread inputs the number of iterations as an arguement. It then initializes the digit representing the current char that is to be stored in the next block, starting at 0. Within the loop, first the producer sleeps for 0-10 microseconds, and then stores the block to be added next in the _input_ array, before iterating digit by 1 and then taking the modulus by 10, as digit should never be greater than 9. It then prints what is about to be enqueued before calling enequeue_buffer_421, pausing the semaphore before printing the number of elements in the buffer.

consumer: The consumer thread inputs the number of iterations as an arguement. Within the loop, first the producer sleeps for 0-10 microseconds, and then calls the dequeue_buffer_421 function to store the dequeued block in the _output_ array. It then pauses the semaphore until the dequeue is coimpleted before printing _output_ and the number of elements remaining in the buffer.

test file(s): The test file initializes two threads, one for the producer and one for the consumer. After initializing the buffer, the test file then creates two concurrently running threads, one for the producer and one for the consumer, and then waits for both threads to finish before deleting the buffer and exiting. test.c is the test file for the kernel implmentation of the buffer, while test_user.c is the test file for the user implmentation of the buffer. The included Makefile is for compiling buffer_user and test_user.
