#include <stdio.h>
#include <ctype.h>
#include <pthread.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif


static void *firstMunch(struct Queue *que);
static void *secondMunch(struct Queue *que);
static void *writeOutput(struct Queue *que);
static void *readStream();
static int checkSizie();

int main(void){
	pthread_t reader, munch1, munch2, writer;
	struct Queue *munch1Q;
	struct Queue *munch2Q;
	struct Queue *writerQ;

	//will have to edit this around, readStream should return an updated char array
	pthread_create(&reader, NULL, readStream, &args);
	pthread_create(&munch1, NULL, firstMunch, munch1Q);
	pthread_create(&munch2, NULL, secondMunch, munch2Q);
	pthread_create(&writer, NULL, writeOutput, writerQ);

	//when finished
	PrintQueueStats();
}

void *readStream(){
}

void *firstMunch(struct Queue *que){

}

void *secondMunch(struct Queue *que){

}

void *writeOutput(struct Queue *que){

}
