#include "queue.h"
#include <stdio.h>
#include <stdlib.h>


struct Queue *CreateStringQueue(unsigned int capacity){
	struct Queue *newQueue = (struct Queue*) malloc(sizeof(struct Queue));
	if(newQueue == NULL){
		fprintf(stderr, "Failed to malloc queue");
		exit(-1); //Exit < 0 on any error
	}
	//Add checks to all of these + malloc with char *
	if(sem_init(&(newQueue->mutex), 0, 1)){
		fprintf(stderr, "Failed to init mutex in createStringQueue");
		exit(-1);
	}
	if(sem_init(&(newQueue->tailLock), 0, 0)){
		fprintf(stderr, "Failed to init tailLock in createStringQueue");
		exit(-1);
	}

	if(sem_init(&(newQueue->headLock), 0, 0)){
		fprintf(stderr, "Failed to init headLock in createStringQueue");
		exit(-1);
	}

	newQueue->lines = malloc(sizeof(char*) * capacity);
	if(newQueue->lines == NULL){
		fprintf(stderr, "Failed to malloc char lines in CreateStringQueue");
		exit(-1);
	}

	newQueue->head = 0;
	newQueue->foot = capacity - 1;
	newQueue->size = 0;
	newQueue->capacity = capacity;

	newQueue->enqueueCount = 0;
	newQueue->dequeueCount = 0;
	newQueue->enqueueBlockCount = 0;
	newQueue->dequeueBlockCount = 0;
	return newQueue;

}

int IsFull(struct Queue *queue) {
    return (queue->size == queue->capacity);
}

int IsEmpty(struct Queue *queue) {
    return (queue->size == 0);
}

void EnqueueString(struct Queue *queue, char *string) {
    //implement locks
    if(sem_wait(&(queue->mutex))){
	fprintf(stderr, "Failed to lock mutex in EnqueueString");
	exit(-1);
    }
    
    if(IsFull(queue)) {
        queue->enqueueBlockCount = queue->enqueueBlockCount + 1;

	if(sem_post(&(queue->mutex))){
		fprintf(stderr, "Failed to unlock mutex in EnqueueString, if(isFull)");
		exit(-1);
	}

	if(sem_wait(&(queue->tailLock))){
		fprintf(stderr, "Failed to unlock tailLock in EnqueueString, if(isFull)");
		exit(-1);
	}	
    }
    
    queue->foot = (queue->foot + 1) % queue->capacity;
    
    queue->lines[queue->foot] = string;
    
    queue->size = queue->size + 1;
    queue->enqueueCount = queue->enqueueCount + 1;
    
    if(sem_post(&(queue->headLock))){
	fprintf(stderr, "Failed to unlock headLock in EnqueueString");
	exit(-1);	
    }
    if(sem_post(&(queue->mutex))){
	fprintf(stderr, "Failed to unlock mutex in EnqueueString");
	exit(-1);
    }
}

//TODO ADD RETURN VALUE IF STATEMENTS FOR ALL SEM STATEMENTS
char* DequeueString(struct Queue *queue) {
    //implement locks
    if(sem_wait(&(queue->mutex))){
	fprintf(stderr, "Failed to lock mutex in EnqueueString");
	exit(-1);
    }


    if (IsEmpty(queue)) {
        queue->dequeueBlockCount = queue->dequeueBlockCount + 1;
    
	if(sem_wait(&(queue->mutex))){
		fprintf(stderr, "Failed to unlock mutex in DequeueString: isEmpty");
		exit(-1);
    	}

	if(sem_wait(&(queue->headLock))){		
		fprintf(stderr, "Failed to unlock headLock in DequeueString: isEmpty");
		exit(-1);
	}
        // BLOCK UNTIL THERE IS A LINE TO REMOVE
    }

    char* line = queue->lines[queue->head];
    queue->head = (queue->head + 1) % queue->capacity;
    queue->size = queue->size - 1;
    queue->dequeueCount = queue->dequeueCount + 1;
    

    if(sem_post(&(queue->tailLock))){
		fprintf(stderr, "Failed to unlock tailLock in DequeueString");
		exit(-1);
    }
    if(sem_post(&(queue->mutex))){
		fprintf(stderr, "Failed to unlock mutex in DequeueString: isEmpty");
		exit(-1);
    }
    return line;
}

void PrintQueueStats(struct Queue* queue) {
    printf("Successful enqueues: %u\n", queue->enqueueCount);
    printf("Successful dequeues: %u\n", queue->dequeueCount);
    printf("Unsuccessful enqueues: %u\n", queue->enqueueBlockCount);
    printf("Unsuccessful dequeues: %u\n", queue->dequeueBlockCount);
}


