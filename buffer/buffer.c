#include <linux/kernel.h>
#include <linux/syscalls.h>
#include "buffer.h"
#include <linux/semaphore.h>

static ring_buffer_421_t buffer;

//Define Semaphores
struct semaphore mutex;
struct semaphore fill_count;
struct semaphore empty_count;

SYSCALL_DEFINE0(init_buffer_421) {
        //Declaring Variables
        node_421_t* node = NULL;
        node_421_t* curr = NULL;
        int i = 0;
        //Check if buffer is initizalzied
        if(buffer.read||buffer.write){
                return -1;
	}
        //Create the root node
        node = (node_421_t*) kmalloc(sizeof(node_421_t),GFP_KERNEL);
        curr = node;
        //Create the rest of the nodes
        for(i = 1; i <SIZE_OF_BUFFER;i++){
                curr->next = (node_421_t*) kmalloc(sizeof(node_421_t),GFP_KERNEL);
                curr = curr->next;
        }
        //Complete the chain
        curr->next = node;
        buffer.read = node;
        buffer.write = node;
        buffer.length = 0;
        //Initizalize Semaphores
        sema_init(&mutex,1);
        sema_init(&fill_count,0);
        sema_init(&empty_count,20);
        return 0;
}


SYSCALL_DEFINE1(enqueue_buffer_421,char*,data){
        long check = 0;
        //Check if buffer is initialized
        if(!buffer.write){
                return -1;
        }
        //Lock the critical section
        down(&empty_count);
        down(&mutex);
        check = copy_from_user(buffer.write->data,data,DATA_LENGTH);
        if(check != 0) return -1;
        //Advance the pointer
        buffer.write = buffer.write->next;
        buffer.length++;
        //Unlock the critical section
        up(&fill_count);
        up(&mutex);
        return 0;
}

SYSCALL_DEFINE1(dequeue_buffer_421,char*,data){
        int check = 0;
        //Check if buffer is initizalized
        if(!buffer.write){
                return -1;
        }
        //Lock the critical section
        down(&fill_count);
        down(&mutex);
        check = copy_to_user(data,buffer.read->data,DATA_LENGTH);
        if(check != 0) return -1;
        //Advance the buffer
        buffer.read = buffer.read->next;
        buffer.length--;
        up(&empty_count);
        up(&mutex);
        return 0;
}

SYSCALL_DEFINE0(delete_buffer_421){
        //Initialize Variables
        node_421_t* temp = NULL;
        node_421_t* curr = NULL;

        //Check if buffer is initialized
        if(!buffer.write){
                return -1;
        }
        //Get rid of all existing nodes
        down(&mutex);
        curr = buffer.read->next;
        while(curr != buffer.read) {
                temp = curr->next;
                kfree(curr);
                curr = temp;
        }
        //Free the final node
        kfree(curr);
        curr = NULL;
        //Reset the Buffer
        buffer.read = NULL;
        buffer.write = NULL;
        buffer.length = 0;

        //Unlock Critical space
        up(&mutex);
        return 0;
}
