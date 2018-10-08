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
static void checkSize();
static int checkLineSize();

int main(void){
	
	pthread_t reader, munch1, munch2, writer;
	struct Queue *munch1Queue;
	struct Queue *munch2Queue;
	struct Queue *writerQueue;

	//will have to edit this around, readStream should return an updated char array
	int createReader = pthread_create(&reader, NULL, readStream, /*notsureyet*/);
	threadCreateCheck(createReader);

	int createMunch1 = pthread_create(&munch1, NULL, firstMunch, munch1Queue);
	threadCreateCheck(createMunch1);

	int createMucnh2 = pthread_create(&munch2, NULL, secondMunch, munch2Queue);
	threadCreateCheck(createMunch2);

	int createWriter = pthread_create(&writer, NULL, writeOutput, writerQueue);
	threadCreateCheck(createWriter);

//	Following code is to wait for when reads are finished, might have to rearrange them
//	pthread_join(reader, /*stuff got passed into reader*/);
//	pthread_join(munch1, (void **) munch1Queue);
//	pthread_join(munch2, (void **) munch2Queue);
//	pthread_join(writer, (void **) writerQueue);

	//when finished
	PrintQueueStats();
}

void *readStream(void * stream){
	/* psuedocode
	 *
	 * check line, if too big skip to next
	 * otherwise read from file and keep queueing up?
	 *
	 */
}

void *firstMunch(void *queue){

}

void *secondMunch(void *queue){

}

void *writeOutput(void *queue){
	struct Queue* queue = (struct * Queue) queue;
	while(!IsEmpty(queue)){	
		char *printMe = DequeueString(que);
		printf("%s\n", printMe);	
	}
}

void threadCreateCheck(int val){
	if(val != 0){
		printf("Failed thread creation \n");
		exit(-1);
	}
}

int checkLineSize(){

}
