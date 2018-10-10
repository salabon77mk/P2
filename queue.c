#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>


struct Queue *CreateStringQueue(unsigned int capacity){

	struct Queue *newQueue = malloc(sizeof(struct Queue *));
	if(newQueue == NULL){
		fprintf(stderr, "Failed to malloc queue");
		exit(0);
	}

	int test0 = sem_init(&(newQueue->mutex), 0, 1);
	int test1 = sem_init(&(newQueue->tailLock), 0, 0);
	int test2 = sem_init(&(newQueue->headLock), 0, 0);
	
	int test3 = sem_wait(&(newQueue->mutex));

	newQueue->lines = malloc(sizeof(char*) * capacity);
	for(unsigned int i = 0; i < capacity; i++){
		newQueue->lines[i] = malloc(sizeof(char*));
	}

	newQueue->head = 0;
	newQueue->foot = capacity - 1;
	newQueue->size = 0;
	newQueue->capacity = capacity;
	newQueue->enqueueCount = 0;
	newQueue->dequeueCount = 0;
	newQueue->enqueueBlockCount = 0;
	newQueue->dequeueBlockCount = 0;

	

	int test4 = sem_post(&(newQueue->mutex));
	return newQueue;

}

int IsFull(struct Queue* queue) {
    return (queue->size == queue->capacity);
}

int IsEmpty(struct Queue* queue) {
    return (queue->size == 0);
}

void EnqueueString(struct Queue* queue, char* string) {
    //implement locks
    int test0 = sem_wait(&(queue->mutex));
    if(IsFull(queue)) {
        queue->enqueueBlockCount = queue->enqueueBlockCount + 1;
	sem_post(&(queue->mutex));
        sem_wait(&(queue->tailLock));
       	// BLOCK UNTIL SPACE AVAILABLE
	
    }
    queue->foot = (queue->foot + 1) % queue->capacity;
    queue->lines[queue->foot] = string;
    queue->size = queue->size + 1;
    queue->enqueueCount = queue->enqueueCount + 1;
    int test1 = sem_post(&(queue->headLock));
    int test2 = sem_post(&(queue->mutex));
	printf("MADE IT TO ENQUEUE \n");
}

char* DequeueString(struct Queue* queue) {
    //implement locks
    sem_wait(&queue->mutex);
    if (IsEmpty(queue)) {
        queue->dequeueBlockCount = queue->dequeueBlockCount + 1;
	sem_post(&(queue->mutex));
	sem_wait(&(queue->headLock));
        // BLOCK UNTIL THERE IS A LINE TO REMOVE
    }
    char* line = queue->lines[queue->head];
    queue->head = (queue->head + 1) % queue->capacity;
    queue->size = queue->size - 1;
    queue->dequeueCount = queue->dequeueCount + 1;
    sem_post(&(queue->tailLock));
    sem_post(&(queue->mutex));
    return line;
}

void PrintQueueStats(struct Queue* queue) {
    printf("Successful enqueues: %u\n", queue->enqueueCount);
    printf("Successful dequeues: %u\n", queue->dequeueCount);
    printf("Unsuccessful enqueues: %u\n", queue->enqueueBlockCount);
    printf("Unsuccessful dequeues: %u\n", queue->dequeueBlockCount);
}
