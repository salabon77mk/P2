#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>



struct Queue *CreateStringQueue(unsigned int capacity){
	capacity = 10; //For	testing

	sem_init(&(newQueue->mutex), NULL, 1);
	sem_init(&(newQueue->tailLock), NULL, 1);
	sem_init(&(newQueue->headLock), NULL, 1);
	
	sem_wait(&(newQueue->mutex));

	struct *newQueue = malloc(sizeof(struct Queue *));
	newQueue->lines = (char*) malloc(sizeof(char) * capacity);

	newQueue->head = 0;
	newQueue->foot = capacity - 1;
	newQueue->size = 0;
	newQueue->capacity = capacity;
	newQueue->enqueueCount = 0;
	newQueue->dequeueCount = 0;
	newQueue->enqueuedBlockCount = 0;
	newQueue->dequeueBlockCount = 0;

	newQueue->queueState = 0; //It's free

	sem_post(&(newQueue->mutex));
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
    sem_wait(&(queue->mutex));
    if(isFull(queue)) {
        queue->enqueueBlockCount = queue->enqueueBlockCount + 1;
	sem_post(&(queue->mutex));
        sem_wait(&(queue->tailLock));
       	// BLOCK UNTIL SPACE AVAILABLE
	
    }
    queue->foot = (queue->foot + 1) % queue->capacity;
    queue->lines[queue->foot] = string;
    queue->size = queue->size + 1;
    queue->enqueueCount = queue->enqueueCount + 1;
    sem_post(&(queue->headLock));
    sem_post(&(queue->mutex));
}

char* DequeueString(struct Queue* queue) {
    //implement locks
    sem_wait(&queue->mutex);
    if (isEmpty(queue)) {
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
    printf("Successful enqueues: %d\n", queue->enqueueCount);
    printf("Successful dequeues: %d\n", queue->dequeueCount);
    printf("Unsuccessful enqueues: %d\n", queue->enqueueBlockedCount);
    printf("Unsuccessful dequeues: %d\n", queue->dequeueBlockCount);
}
