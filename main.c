#include <stdio.h>
#include <ctype.h>
#include <pthread.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

//TODO make struct to hold args for readStream?

static void *firstMunch(void *queue);
static void *secondMunch(void *queue);
static void *writeOutput(void *queue);
static void *readStream(void *stream);
static int checkSizie();

int main(void){
	
	pthread_t reader, munch1, munch2, writer;
	struct Queue *munch1Q;
	struct Queue *munch2Q;
	struct Queue *writerQ;

	//will have to edit this around, readStream should return an updated char array
	int createReader = pthread_create(&reader, NULL, readStream, /*notsureyet*/);
	int createMunch1 = pthread_create(&munch1, NULL, firstMunch, munch1Q);
	int createMucnh2 = pthread_create(&munch2, NULL, secondMunch, munch2Q);
	int createWriter = pthread_create(&writer, NULL, writeOutput, writerQ);

	//when finished
	PrintQueueStats();
}

void *readStream(void * stream){
}

void *firstMunch(void *queue){

}

void *secondMunch(void *queue){

}

void *writeOutput(void *queue){
	char *printMe = DequeueString(que);
	printf("%s\n", printMe);	
}
