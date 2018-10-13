#include <pthread.h>
#include <stdio.h>
#include "queue.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 64
#endif

#ifndef CAPACITY
#define CAPACITY 10
#endif

struct Queue_Tuple {
    struct Queue* queue1;
    struct Queue* queue2;
};

static void *firstMunch(void *tuple);
static void *secondMunch(void *tuple);
static void *writeOutput(void *queue);
static void *readStream(void *queue);
static void threadRetValCheck(int val);
static struct Queue_Tuple *makeTuple(struct Queue *queue1, struct Queue *queue2);

int main(void){
	struct Queue *munch1Queue = CreateStringQueue(CAPACITY);
	struct Queue *munch2Queue = CreateStringQueue(CAPACITY);
	struct Queue *writerQueue = CreateStringQueue(CAPACITY);

	struct Queue_Tuple *munch1_tuple = makeTuple(munch1Queue, munch2Queue);
	struct Queue_Tuple *munch2_tuple = makeTuple(munch2Queue, writerQueue);
	
	pthread_t reader, munch1, munch2, writer;

	int createReader = pthread_create(&reader, NULL, readStream, munch1Queue);
	threadRetValCheck(createReader);
	
	int createMunch1 = pthread_create(&munch1, NULL, firstMunch, munch1_tuple);
	threadRetValCheck(createMunch1);

	int createMunch2 = pthread_create(&munch2, NULL, secondMunch, munch2_tuple);
	threadRetValCheck(createMunch2);

	int createWriter = pthread_create(&writer, NULL, writeOutput, writerQueue);
	threadRetValCheck(createWriter);


	int joinReader = pthread_join(reader, NULL);
	threadRetValCheck(joinReader);

	int joinMunchOne = pthread_join(munch1, NULL);
	threadRetValCheck(joinMunchOne);

	int joinMunchTwo = pthread_join(munch2, NULL);
	threadRetValCheck(joinMunchTwo);

	int joinWriter = pthread_join(writer, NULL);
	threadRetValCheck(joinWriter);

	PrintQueueStats(munch1Queue);
	PrintQueueStats(munch2Queue);
	PrintQueueStats(writerQueue);
	
}

void *readStream(void *queue){

	struct Queue *q = queue;
	char *str = (char *) malloc(sizeof(char) * BUFFER_SIZE);
	if(str == NULL){
		fprintf(stderr, "Malloc failed on str in readStream");
		exit(-1);
	}

	int currChar;
	int counter = 0;
	while((currChar = fgetc(stdin)) != EOF){
		if(counter >= BUFFER_SIZE - 1){
			fprintf(stderr, "That line is too long, flushing\n");
			counter = 0;
			memset(str, '\0', BUFFER_SIZE);
			while ((currChar = fgetc(stdin)) != '\n'  && currChar != EOF);
		}

		// Reached new line char, time to enqueue
		else if(currChar == '\n'){
			str[counter] = currChar;
			str[counter + 1] = '\0';
//			printf("INPUT: %s", str);
			EnqueueString(q, str);
			counter = 0;
			str = (char *) malloc(sizeof(char) * BUFFER_SIZE);
			if(str == NULL){
				fprintf(stderr, "Fail malloc in readStrea: else if");
				exit(-1);
			}
		}
		//Filling out the array index by index
		else{
			str[counter] = currChar;
			counter++;
		}
	}
	EnqueueString(q, NULL);
	//Fixes potential memory leak warning from clang static analyzer
	free(str);

	return NULL;
}

void *firstMunch(void *tuple){

    struct Queue_Tuple *tup = tuple;
    char *line = DequeueString(tup->queue1);

    while(line != NULL){
        char *idx = strchr(line, ' ');
        while (idx) {
            *idx = '*';
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
    char *line = DequeueString(tup->queue1);

    while(line != NULL) {
        for(unsigned int i = 0; i < strlen(line); i++){
            int up = toupper(line[i]);
            line[i] = up;
        }
		
	EnqueueString(tup->queue2, line);
        line = DequeueString(tup->queue1);
    }
    EnqueueString(tup->queue2, line);
    return NULL;
}


void *writeOutput(void *queue){

	struct Queue *writeQueue = queue;
	char *printMe = DequeueString(writeQueue);

	while(printMe != NULL){
		printf("Output: %s", printMe);
		free(printMe);
		printMe = NULL;
		printMe = DequeueString(writeQueue);
	}

	return NULL;
}

void threadRetValCheck(int val){

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
	return qtup;
}
