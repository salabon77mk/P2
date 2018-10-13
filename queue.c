#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


struct Queue *CreateStringQueue(unsigned int capacity){
	struct Queue *newQueue = (struct Queue*) malloc(sizeof(struct Queue));
	if(newQueue == NULL){
		fprintf(stderr, "Failed to malloc queue");
		exit(-1); //Exit < 0 on any error
	}

        pthread_mutex_init(&newQueue->mutex, NULL);	
	pthread_cond_init(&newQueue->tailLock, NULL);
	pthread_cond_init(&newQueue->headLock, NULL);

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
    return (queue->size == queue->capacity);
}

int IsEmpty(struct Queue *queue) {
	return queue->size == 0;
}

void EnqueueString(struct Queue *queue, char *string) {
	pthread_mutex_lock(&queue->mutex);
    while(IsFull(queue)) {
        queue->enqueueBlockCount = queue->enqueueBlockCount + 1;
	pthread_cond_wait(&queue->tailLock, &queue->mutex);
    } 

    queue->tail = (queue->tail + 1) % queue->capacity;
    queue->lines[queue->tail] = string;
    queue->size = queue->size + 1;
    queue->enqueueCount = queue->enqueueCount + 1;
    pthread_cond_signal(&(queue->headLock));
    pthread_mutex_unlock(&queue->mutex);
    
}

char* DequeueString(struct Queue *queue) {
    pthread_mutex_lock(&queue->mutex);

     while(IsEmpty(queue)) {
        queue->dequeueBlockCount = queue->dequeueBlockCount + 1;
	pthread_cond_wait(&queue->headLock, &queue->mutex);
    }

    char* line = queue->lines[queue->head];
    queue->head = (queue->head + 1) % queue->capacity;
    queue->size = queue->size - 1;
    queue->dequeueCount = queue->dequeueCount + 1;
    pthread_cond_signal(&(queue->tailLock));
    pthread_mutex_unlock(&queue->mutex);
    return line;
}

void PrintQueueStats(struct Queue* queue) {
    printf("Successful enqueues: %u\n", queue->enqueueCount);
    printf("Successful dequeues: %u\n", queue->dequeueCount);
    printf("Unsuccessful enqueues: %u\n", queue->enqueueBlockCount);
    printf("Unsuccessful dequeues: %u\n", queue->dequeueBlockCount);
}


