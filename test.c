#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

#define __NR_init_buffer_421 442
#define __NR_enqueue_buffer_421 443
#define __NR_dequeue_buffer_421 444
#define __NR_delete_buffer_421 445

int items = 0;
static sem_t mutex;

long init_buffer_421(void) {
    return syscall(__NR_init_buffer_421);
}
long enqueue_buffer_421(int i) {
    return syscall(__NR_enqueue_buffer_421, i);
}
long dequeue_buffer_421(void) {
    return syscall(__NR_dequeue_buffer_421);
}
long delete_buffer_421(void) {
    return syscall(__NR_delete_buffer_421);
}

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
    pthread_t enqueue,dequeue;
    sem_init(&mutex,0,1);
    init_buffer_421();
    int iter_count = 100000;
    //Create Threads
    pthread_create(&enqueue,NULL,producer,&iter_count);
    pthread_create(&dequeue,NULL,consumer,&iter_count);
    //Wait for Threads to finish
    pthread_join(enqueue,NULL);
    pthread_join(dequeue,NULL);
    delete_buffer_421();
    return 0;
}
