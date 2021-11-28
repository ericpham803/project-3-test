#include "buffer.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

int items = 0;
static sem_t mutex;

//Producer
void* producer(void* value){
    int* iter_count = (int* )value; 
    int digit = 0;
    char input[DATA_LENGTH];
    for(int i = 0; i < *iter_count;i++){
        usleep((rand() % 10) * 1000); //0-10 milliseconds = 0-10000 microseconds

        for(int j = 0; j < DATA_LENGTH ; j++)
            input[j] = digit + '0';
        digit = (digit + 1) % 10;

        printf(":: Enqueueing element into buffer. ::\n");
        printf("%.10s...\n", input);
        enqueue_buffer_421(input);
        sem_wait(&mutex);
        printf("%i items in buffer.\n", ++items);
        sem_post(&mutex);
    }
}
//Consumer
void* consumer(void* value){
    int* iter_count = (int*) value;
    char output[DATA_LENGTH];
    for(int i = 0; i < *iter_count;i++){
        usleep((rand() % 10) * 1000); //0-10 milliseconds = 0-10000 microseconds

        printf(":: Dequeuing element into buffer. ::\n");
        dequeue_buffer_421(output);
        sem_wait(&mutex);
        printf("%.10s...\n", output);
        printf("%i items in buffer.\n", --items);
        sem_post(&mutex);
    }
}

int main(){
    pthread_t enqueue_thread,dequeue_thread;
    sem_init(&mutex,0,1);
    init_buffer_421();
    int iter_count = 1000;
    //Create Threads
    pthread_create(&enqueue_thread,NULL,producer,&iter_count);
    pthread_create(&dequeue_thread,NULL,consumer,&iter_count);
    //Wait for Threads to finish
    pthread_join(enqueue_thread,NULL);
    pthread_join(dequeue_thread,NULL);
    delete_buffer_421();
    return 0;
}
