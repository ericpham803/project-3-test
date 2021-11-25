#include "buffer.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
char data [DATA_LENGTH];
char input [DATA_LENGTH];
int items = 0;

static sem_t mutex;
//Producer
void* producer(void* value){
    int* amount = (int* )value;
    int time = 0;
    for(int i = 0; i < *amount;i++){
	for(int j = 0; j < DATA_LENGTH ; j++){
	    input[j] = i % 10 + '0';
	}
	time = rand() % 1000;
	usleep((__useconds_t)time);
	enqueue_buffer_421(input);
	sem_wait(&mutex);
	printf(":: Enqueueing element into buffer. ::\n");
	printf("%.10s...\n", input);
	printf("%i items in buffer.\n", ++items);
	sem_post(&mutex);
    }
}
//Consumer
void* consumer(void* value){
    int* amount = (int*) value;
    int time = 0;
    for(int i = 0; i < *amount;i++){
        time = rand() % 1000;
        usleep((__useconds_t)time);
	dequeue_buffer_421(data);
	sem_wait(&mutex);
	printf(":: Dequeuing element into buffer. ::\n");
        printf("%.10s...\n", data);
        printf("%i items in buffer.\n", --items);
	sem_post(&mutex);
    }
}

int main(){
    pthread_t enqueue,dequeue;
    sem_init(&mutex,0,1);y
    init_buffer_421();
    int amount = 1000;
    //Create Threads
    pthread_create(&enqueue,NULL,producer,&amount);
    pthread_create(&dequeue,NULL,consumer,&amount);
    //Wait for Threads to finish
    pthread_join(enqueue,NULL);
    pthread_join(dequeue,NULL);
    delete_buffer_421();
    return 0;
}
