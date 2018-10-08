#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

struct Queue {
    int head, foot, size;
    unsigned int capacity;
    unsigned int enqueueCount;
    unsigned int dequeueCount;
    unsigned int enqueueBlockCount;
    unsigned int dequeueBlockCount;
    char* lines;
};


struct Queue *CreateStringQueue(unsigned int capacity){
	size = 10; //For	testing

	/** REFERENCING https://bit.ly/2pFGWKJ
     *
        struct Queue* queue = (struct Queue*) malloc(sizeof(struct Queue));
        queue->capacity = size;
        queue->head = 0;
        queue->foot = capacity - 1;
        queue->size = 0;
        queue->enqueueCount = 0;
        queue->dequeueCount = 0;
        queue->enqueueBlockCount = 0;
        queue->dequeueBlockCount = 0;
        queue->lines = (char*) malloc(queue->capacity * sizeof(char));
        return queue;
	 */

	struct *newQueue = malloc(sizeof(struct Queue *));
	newQueue->strings = (char*) malloc(sizeof(char) * size);

	newQueue->enqueueCount = 0;
	newQueue->dequeueCount = 0;
	newQueue->enqueuedBlockCount = 0;
	newQueue->dequeueBlockCount = 0;

	newQueue->queueState = 0; //It's free
	sem_init(&(newQueue->mutex), NULL, 1);
	sem_init(&(newQueue->ready), NULL, 1);

	return newQueue;

}

int IsFull(Queue* queue) {
    return (queue->size == queue->capacity);
}

int IsEmpty(Queue* queue) {
    return (queue->size == 0);
}

void EnqueueString(Queue* queue, char* string) {
    if(isFull(queue)) {
        queue->enqueueBlockCount = queue->enqueueBlockCount + 1;
        // BLOCK UNTIL SPACE AVAILABLE
    }
    queue->foot = (queue->foot + 1)%queue->capacity;
    queue->lines[queue->foot] = string;
    queue->size = queue->size + 1;
    queue->enqueueCount = queue->enqueueCount + 1;
}

char* DequeueString(Queue* queue) {
    if (isEmpty(queue)) {
        queue->dequeueBlockCount = queue->dequeueBlockCount + 1;
        // BLOCK UNTIL THERE IS A LINE TO REMOVE
    }
    char* line = queue->lines[queue->head];
    queue->head = (queue->head + 1)%queue->capacity;
    queue->size = queue->size - 1;
    queue->dequeueCount = queue->dequeueCount + 1;
    return line;
}

void PrintQueueStats(Queue* queue) {
    printf("Successful enqueues: %d\n", queue->enqueueCount);
    printf("Successful dequeues: %d\n", queue->dequeueCount);
    printf("Unsuccessful enqueues: %d\n", queue->enqueueBlockedCount);
    printf("Unsuccessful dequeues: %d\n", queue->dequeueBlockCount);
}
