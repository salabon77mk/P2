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
static void *readStream(void *queue);
static void threadCreateCheck(int val);
static struct Queue_Tuple *makeTuple(struct Queue *queue1, struct Queue *queue2);

int main(void){
	//TODO Edit struct to hold some boolean once reader has read EOF

	//create queues
	struct Queue *munch1Queue = CreateStringQueue(CAPACITY);
	struct Queue *munch2Queue = CreateStringQueue(CAPACITY);
	struct Queue *writerQueue = CreateStringQueue(CAPACITY);


	struct Queue_Tuple *munch1_tuple = makeTuple(munch1Queue, munch2Queue);
	struct Queue_Tuple *munch2_tuple = makeTuple(munch2Queue, writerQueue);
	//threads
	pthread_t reader, munch1, munch2, writer;

	int createReader = pthread_create(&reader, NULL, readStream, munch1Queue);
	threadCreateCheck(createReader);


	int createMunch1 = pthread_create(&munch1, NULL, firstMunch, munch1_tuple);
	threadCreateCheck(createMunch1);

	int createMunch2 = pthread_create(&munch2, NULL, secondMunch, munch2_tuple);
	threadCreateCheck(createMunch2);

	int createWriter = pthread_create(&writer, NULL, writeOutput, writerQueue);
	threadCreateCheck(createWriter);


	printf("CREATED ALL THREADS! \n");
	//Following code is to wait for when reads are finished, might have to rearrange them
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
	char *str;
	str = (char *) malloc(sizeof(char) * BUFFER_SIZE);

	if(str == NULL){
		fprintf(stderr, "Malloc failed on str in readStream");
		exit(-1);
	}

	int currChar;
	int counter = 0;
	while((currChar = fgetc(stdin)) != EOF){
		if(counter >= BUFFER_SIZE){
			fprintf(stderr, "That line is too long, flushing");
			counter = 0;
			int ch;
			while ((ch = fgetc(stdin)) != '\n'  && ch != EOF);
		}

		// Reached new line char, time to enqueue
		else if(currChar == '\n'){
			str[counter] = currChar;
			EnqueueString(q, str);
			printf("%s \n", str);
			counter = 0;
			str = (char *) malloc(sizeof(char) * BUFFER_SIZE);
		}
		else{
			str[counter] = currChar;
			counter++;
		}
	}
	EnqueueString(q, NULL);

	return NULL;
}

void *firstMunch(void *tuple){

    struct Queue_Tuple *tup = tuple;
    char *line = DequeueString(tup->queue1);

    while(line != NULL){
        char *idx = strchr(line, ' ');
        while (idx) {
            idx = '*';
            idx = strchr(idx + 1, ' ');
        }
        EnqueueString(tup->queue2, line);

	line = DequeueString(tup->queue1);
    }
    EnqueueString(tup->queue2, line);
    return NULL;
}

void *secondMunch(void *tuple){
    struct Queue_Tuple *tup = tuple;
    char *line = DequeueString(queue1);

    while(line != NULL) {
        int i = 0;
        while (line[i]) {
            putchar(toupper(line[i]));
            i++;
        }
        line = DequeueString(tup->queue2, line);
    }
    EnqueueString(tup->queue2, line);
    return NULL;
}


void *writeOutput(void *queue){

	struct Queue *writeQueue = tuple;
	char *printMe = DequeString(writeQueue);

	while(printMe != NULL){

	}


void threadCreateCheck(int val){
	if(val != 0){
		fprintf(stderr, "Failed thread creation \n");
		exit(-1);
	}
}


struct Queue_Tuple *makeTuple(struct Queue *queue1, struct Queue *queue2){
	struct Queue_Tuple *qtup = (struct Queue_Tuple*) malloc(sizeof(struct Queue_Tuple));
	if(qtup == NULL){
		fprintf(stderr, "Malloc failed at queue creation\n");
		exit(-1);
	}
	qtup->queue1 = queue1;
	qtup->queue2 = queue2;
	qtup->reachedEOF = 0; //false
	return qtup;
}
