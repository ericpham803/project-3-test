#include <stdlib.h>
#include <stdio.h>
#include "buffer.h"

static ring_buffer_421_t buffer;
static sem_t mutex;
static sem_t fill_count;
static sem_t empty_count;
static int digit;

long init_buffer_421(void) {
	// Note: You will need to initialize semaphores in this function.
	// Ensure we're not initializing a buffer that already exists.
	if (buffer.read || buffer.write) {
		printf("init_buffer_421(): Buffer already exists. Aborting.\n");
		return -1;
	}

	// Create the root node.
	node_421_t *node;
	node = (node_421_t *) malloc(sizeof(node_421_t));
	// Create the rest of the nodes, linking them all together.
	node_421_t *current;
	int i;
	current = node;
	// Note that we've already created one node, so i = 1.
	for (i = 1; i < SIZE_OF_BUFFER; i++) {
		current->next = (node_421_t *) malloc(sizeof(node_421_t));
		current = current->next;
	}
	// Complete the chain.
	current->next = node;
	buffer.read = node;
	buffer.write = node;
	buffer.length = 0;

	digit = 0;
	// Initialize your semaphores here.
	// TODO

	return 0;
}

long enqueue_buffer_421(char * data) {
	// NOTE: You have to modify this function to use semaphores.
	if (!buffer.write) {
		printf("write_buffer_421(): The buffer does not exist. Aborting.\n");
		return -1;
	}
	memcpy(buffer.write->data, data, DATA_LENGTH);
	// Advance the pointer.
	buffer.write = buffer.write->next;
	buffer.length++;

	return 0;
}

long dequeue_buffer_421(char * data) {
	// NOTE: Implement this function.
	return 0;
}

long delete_buffer_421(void) {
	// Tip: Don't call this while any process is waiting to enqueue or dequeue.
	if (!buffer.read) {
		printf("delete_buffer_421(): The buffer does not exist. Aborting.\n");
		return -1;
	}
	// Get rid of all existing nodes.
	node_421_t *temp;
	node_421_t *current = buffer.read->next;
	while (current != buffer.read) {
		temp = current->next;
		free(current);
		current = temp;
	}
	// Free the final node.
	free(current);
	current = NULL;
	// Reset the buffer.
	buffer.read = NULL;
	buffer.write = NULL;
	buffer.length = 0;
	return 0;
}

void print_semaphores(void) {
	// You can call this method to check the status of the semaphores.
	// Don't forget to initialize them first!
	// YOU DO NOT NEED TO IMPLEMENT THIS FOR KERNEL SPACE.
	int value;
	sem_getvalue(&mutex, &value);
	printf("sem_t mutex = %d\n", value);
	sem_getvalue(&fill_count, &value);
	printf("sem_t fill_count = %d\n", value);
	sem_getvalue(&empty_count, &value);
	printf("sem_t empty_count = %d\n", value);
	return;
}

void producer(void) {
	pthread_t = prod_thread;
	//wait 0-100 milliseconds. usleep counts in microseconds
	usleep((rand() % 101) * 1000);
	//set data to be enqueued
	char* data = malloc(DATA_LENGTH * sizeof(char));
	for(int i = 0; i < DATA_LENGTH; i++)
		data[i] = digit+'0'; 
	//increment digit. variable should be stored gloablly
	digit += 1;
	if(digit > 9)
		digit = 0;
	//print status update
	printf("::Enqueueing element into buffer. ::\n");
	for(int i = 0; i < 10; i++)
		printf("%c", data[0]);
	printf("...\n");
	//call new thread to enqueue data
	pthread_create(&prod_thread, NULL, &enqueue_buffer_421, NULL);
	//TODO: figure out if something goes in between this
	//join basically waits for prod_thread to finish, not sure if this is how concurrency is implemented
	pthread_join(prod_thread);
	//does following code belong in enqueue or here?
	int queue_len;
	sem_getvalue(&fill_count, &queue_len);
	printf("%d items in buffer.\n", queue_len);
}

void consumer(void) {
	pthread_t = cons_thread;
	//wait 0-100 milliseconds. usleep counts in microseconds
	usleep((rand() % 101) * 1000);
	//set data to return value of dequeue
	char* data = pthread_create(&cons_thread, NULL, &dequeue_buffer_421, NULL);
	//TODO: figure out if something goes in between this
	//also figure out what happens if dequeue returns an error
	//join basically waits for prod_thread to finish, not sure if this is how concurrency is implemented
	pthread_join(cons_thread);
	printf(":: Dequeueing element from buffer. ::\n");
	for(int i = 0; i < 10; i++)
		printf("%c",data[i])
	printf("...\n");
	//does following code belong in enqueue or here?
	int queue_len;
	sem_getvalue(&fill_count, &queue_len);
	printf("%d items in buffer.\n", queue_len);
}