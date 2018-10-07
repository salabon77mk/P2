#include <stdio.h>
#include <ctype.h>
#include <pthread.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif




int main(void){
	pthread_t reader, munch1, munch2, writer;
	struct Queue *munch1Queue;
	struct Queue *munch2Queue
	struct Queue *writerQueue;
	//when finished
	PrintQueueStats();
}
