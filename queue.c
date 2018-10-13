#include "queue.h"
#include <stdio.h>
#include <stdlib.h>


struct Queue *CreateStringQueue(unsigned int capacity){
	struct Queue *newQueue = (struct Queue*) malloc(sizeof(struct Queue));
	if(newQueue == NULL){
		fprintf(stderr, "Failed to malloc queue");
		exit(-1); //Exit < 0 on any error
	}

		
	if(sem_init(&(newQueue->mutex), 0, 1)){
		fprintf(stderr, "Failed to init mutex in createStringQueue");
		exit(-1);
	}
	
	
	if(sem_init(&(newQueue->tailLock), 0, 1)){
		fprintf(stderr, "Failed to init tailLock in createStringQueue");
		exit(-1);
	}

	if(sem_init(&(newQueue->headLock), 0, 1)){
		fprintf(stderr, "Failed to init headLock in createStringQueue");
		exit(-1);
	}

	newQueue->lines = malloc(sizeof(char*) * capacity);
	if(newQueue->lines == NULL){
		fprintf(stderr, "Failed to malloc char lines in CreateStringQueue");
		exit(-1);
	}

	newQueue->head = 0;
	newQueue->tail = capacity - 1;
	newQueue->size = 0;
	newQueue->capacity = capacity;

	newQueue->enqueueCount = 0;
	newQueue->dequeueCount = 0;
	newQueue->enqueueBlockCount = 0;
	newQueue->dequeueBlockCount = 0;
	return newQueue;

}

int IsFull(struct Queue *queue) {
	if(queue->size == queue->capacity){
		sem_post(&(queue->headLock));
		return 1;
	}
	
	return 0;
//    return (queue->size == queue->capacity);
}

int IsEmpty(struct Queue *queue) {
	if(queue->size == 0){
		sem_post(&(queue->tailLock));
		return 1;
	}
	return 0;
//    return (queue->size == 0);
}

void EnqueueString(struct Queue *queue, char *string) {
     sem_wait(&(queue->mutex));

    while(IsFull(queue)) {
        queue->enqueueBlockCount = queue->enqueueBlockCount + 1;
        sem_post(&(queue->mutex));
	sem_wait(&(queue->headLock));
    } 

    queue->tail = (queue->tail + 1) % queue->capacity;
    queue->lines[queue->tail] = string;
    queue->size = queue->size + 1;
    queue->enqueueCount = queue->enqueueCount + 1;
    sem_post(&(queue->mutex));
    
}

char* DequeueString(struct Queue *queue) {
     sem_wait(&(queue->mutex));

     while(IsEmpty(queue)) {
        queue->dequeueBlockCount = queue->dequeueBlockCount + 1;
        sem_post(&(queue->mutex));
	sem_wait(&(queue->tailLock));
    }

    char* line = queue->lines[queue->head];
    queue->head = (queue->head + 1) % queue->capacity;
    queue->size = queue->size - 1;
    queue->dequeueCount = queue->dequeueCount + 1;
    sem_post(&(queue->mutex));
    

    return line;
}

void PrintQueueStats(struct Queue* queue) {
    printf("Successful enqueues: %u\n", queue->enqueueCount);
    printf("Successful dequeues: %u\n", queue->dequeueCount);
    printf("Unsuccessful enqueues: %u\n", queue->enqueueBlockCount);
    printf("Unsuccessful dequeues: %u\n", queue->dequeueBlockCount);
}


