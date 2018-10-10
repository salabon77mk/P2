#include <pthread.h>
#include <stdio.h>
#include <ctype.h>
#include "queue.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 10
#endif

#ifndef CAPACITY
#define CAPACITY 10
#endif

//TODO make struct to hold args for readStream?


struct Queue_Tuple {
    struct Queue* queue1;
    struct Queue* queue2;
    unsigned int reachedEOF;
};

static void *firstMunch(void *tuple);
static void *secondMunch(void *tuple);
static void *writeOutput(void *queue);
static void *readStream(void *stream);
static void threadCreateCheck(int val);
static struct Queue_Tuple *makeTuple(struct Queue *queue1, struct Queue *queue2);

int main(void){
	//TODO Edit struct to hold some boolean once reader has read EOF
	
	//create queues
	struct Queue *munch1Queue = CreateStringQueue(CAPACITY);
	struct Queue *munch2Queue = CreateStringQueue(CAPACITY);
	struct Queue *writerQueue = CreateStringQueue(CAPACITY);

	//edit these later for niceness
	
	struct Queue_Tuple *munch1_tuple = makeTuple(munch1Queue, munch2Queue);
	struct Queue_Tuple *munch2_tuple = makeTuple(munch2Queue, writerQueue);
	

	/* Will delete later, this was the original long form
	struct Queue_Tuple *munch1_tuple = malloc(sizeof(struct Queue_Tuple*));
	munch1_tuple->queue1 = munch1Queue;
	munch1_tuple->queue2 = munch2Queue;

	struct Queue_Tuple *munch2_tuple = malloc(sizeof(struct Queue_Tuple*));
	munch2_tuple->queue1 = munch2Queue;
	munch2_tuple->queue2 = writerQueue;
	*/

	//threads
	pthread_t reader, munch1, munch2, writer;
	//create our four threads
	/* alternative style, shouldn't be any different though, only used to see
	 * if stderr is ever reached, it isn't
	if(pthread_create(&reader, NULL, readStream, munch1Queue)){
		fprintf(stderr, "We got some issues with pthread creation");
		return 1;
	}
	*/

	int createReader =  pthread_create(&reader, NULL, readStream, munch1Queue);
	threadCreateCheck(createReader);
	

	int createMunch1 = pthread_create(&munch1, NULL, firstMunch,  munch1_tuple);
	threadCreateCheck(createMunch1);

	int createMunch2 = pthread_create(&munch2, NULL, secondMunch,  munch2_tuple);
	threadCreateCheck(createMunch2);

	int createWriter = pthread_create(&writer, NULL, writeOutput,  writerQueue);
	threadCreateCheck(createWriter);

	printf("CREATED ALL THREADS! \n");
//	Following code is to wait for when reads are finished, might have to rearrange them
	pthread_join(reader, NULL);
	pthread_join(munch1, NULL);
	pthread_join(munch2, NULL);
	pthread_join(writer, NULL);

	//when finished
	PrintQueueStats(munch1Queue);
	PrintQueueStats(munch2Queue);
	PrintQueueStats(writerQueue);
}


//TODO: Change loops in all thread functions, some stuff is placeholder
void *readStream(void *queue){
	struct Queue *q = queue;
	/* psuedocode
	 *
	 * check line, if too big skip to next
	 * otherwise read from file and keep queueing up?
	 *
	 */
	char *str;
	str = calloc(BUFFER_SIZE, sizeof(char));

	if(str == NULL){
		printf("Calloc failed on str");
		exit(-1);
	}


	while(fgets(str, BUFFER_SIZE, stdin) != NULL){
		if(str[BUFFER_SIZE - 1] == '\0' && str[BUFFER_SIZE - 2] != '\n'){
			fprintf(stderr, "That line was too long, flushing to new line");
			int nom;
			while((nom = fgetc(stdin)) != '\n' && nom != EOF); /* munches to the end */
		}
		else{
			EnqueueString(q, str);
		}
	}
	return NULL;
}

void *firstMunch(void *tuple){
    struct Queue_Tuple *tup = tuple;
    while(!IsEmpty(tup->queue1)) {
        char *line = DequeueString(tup->queue1);
        char *idx = strchr(line, ' ');
        while (idx) {
            *idx = '*';
            idx = strchr(idx + 1, ' ');
        }
        EnqueueString(tup->queue2, line);
    }
   return NULL;
}

void *secondMunch(void *tuple){
    struct Queue_Tuple *tup = tuple;
    while(!IsEmpty(tup->queue1)) {
        char* line = DequeueString(tup->queue1);
        int i = 0;
        while(line[i]) {
            putchar(toupper(line[i]));
            i++;
        }
        EnqueueString(tup->queue2, line);
    }
    return NULL;
}

void *writeOutput(void *queue){
	struct Queue *q = queue;
	while(!IsEmpty(q)){
		char *printMe = DequeueString(q);
		printf("%s\n", printMe);
		free(printMe);
	}
	return NULL;
}

void threadCreateCheck(int val){
	if(val != 0){
		fprintf(stderr, "Failed thread creation \n");
		exit(-1);
	}
}


struct Queue_Tuple *makeTuple(struct Queue *queue1, struct Queue *queue2){
	struct Queue_Tuple *qtup = malloc(sizeof(struct Queue_Tuple*));
	if(qtup == NULL){
		fprintf(stderr, "Malloc failed at queue creation\n");
		exit(-1);
	}
	qtup->queue1 = queue1;
	qtup->queue2 = queue2;
	qtup->reachedEOF = 0; //false
	return qtup;
}
