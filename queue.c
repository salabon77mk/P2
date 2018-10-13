#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


struct Queue *CreateStringQueue(unsigned int capacity){
	struct Queue *newQueue = (struct Queue*) malloc(sizeof(struct Queue));
	if(newQueue == NULL){
		fprintf(stderr, "Failed to malloc queue");
		exit(-1); //Exit < 0 on any error
	}

        if(pthread_mutex_init(&newQueue->mutex, NULL)){
		fprintf(stderr, "Failed to initialize mutex");
		exit(-1);
	}
	if(pthread_cond_init(&newQueue->tailLock, NULL)){
		fprintf(stderr, "Failed to initialize cond_t tailLock");
		exit(-1);
	}
	if(pthread_cond_init(&newQueue->headLock, NULL)){
		fprintf(stderr, "Failed to initialize cond_t headLock");
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

static void Pthread_Mutex_Lock(pthread_mutex_t *mutex){
	int retVal = pthread_mutex_lock(mutex);
	assert(retVal == 0);
}

static void Pthread_Mutex_Unlock(pthread_mutex_t *mutex){
	int retVal = pthread_mutex_unlock(mutex);
	assert(retVal == 0);
}

static void Pthread_Cond_Wait(pthread_cond_t *condVar, pthread_mutex_t *mutex){
	int retVal = pthread_cond_wait(condVar, mutex);
	assert(retVal == 0);
}

static void Pthread_Cond_Signal(pthread_cond_t *condVar){
	int retVal = pthread_cond_signal(condVar);
	assert(retVal == 0);
}

static int IsFull(struct Queue *queue) {
    return (queue->size == queue->capacity);
}

static int IsEmpty(struct Queue *queue) {
	return queue->size == 0;
}

void EnqueueString(struct Queue *queue, char *string) {
    Pthread_Mutex_Lock(&queue->mutex);
    while(IsFull(queue)) {
        queue->enqueueBlockCount = queue->enqueueBlockCount + 1;
	Pthread_Cond_Wait(&queue->tailLock, &queue->mutex);
    } 

    queue->tail = (queue->tail + 1) % queue->capacity;
    queue->lines[queue->tail] = string;
    queue->size = queue->size + 1;
    queue->enqueueCount = queue->enqueueCount + 1;
    Pthread_Cond_Signal(&(queue->headLock));
    Pthread_Mutex_Unlock(&queue->mutex);
    
}

char* DequeueString(struct Queue *queue) {
    Pthread_Mutex_Lock(&queue->mutex);

    while(IsEmpty(queue)) {
        queue->dequeueBlockCount = queue->dequeueBlockCount + 1;
	Pthread_Cond_Wait(&queue->headLock, &queue->mutex);
    }

    char* line = queue->lines[queue->head];
    queue->head = (queue->head + 1) % queue->capacity;
    queue->size = queue->size - 1;
    queue->dequeueCount = queue->dequeueCount + 1;
    Pthread_Cond_Signal(&(queue->tailLock));
    Pthread_Mutex_Unlock(&queue->mutex);
    return line;
}

void PrintQueueStats(struct Queue* queue) {
    printf("Successful enqueues: %u\n", queue->enqueueCount);
    printf("Successful dequeues: %u\n", queue->dequeueCount);
    printf("Unsuccessful enqueues: %u\n", queue->enqueueBlockCount);
    printf("Unsuccessful dequeues: %u\n", queue->dequeueBlockCount);
}

