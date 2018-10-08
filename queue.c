#include "queue.h"
#include <semaphore.h>

struct Queue *CreateStringQueue(int size){
	size = 10; //For	testing

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
