#include <stdio.h>
#include <ctype.h>
#include "queue.h"
#include <pthread.h>
#include <string.h>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 1024
#endif

//TODO make struct to hold args for readStream?

static void *firstMunch(void *queue);
static void *secondMunch(void *queue);
static void *writeOutput(void *queue);
static void *readStream(void *stream);
static int checkLineSize();
static void mallocCheck(struct Queue *queue);

struct Queue_Tuple {
    struct Queue* queue1;
    struct Queue* queue2;
};

int main(void){

	pthread_t reader, munch1, munch2, writer;
	struct Queue *munch1Queue = malloc(sizeof(struct *Queue));
	mallocCheck(munch1Queue);

	struct Queue *munch2Queue = malloc(sizeof(struct *Queue));
	mallocCheck(munch2Queue);

	struct Queue *writerQueue = malloc(sizeof(struct *Queue));
	mallocCheck(writerQueue);

	struct Queue_Tuple *munch1_tuple;
	munch1_tuple->queue1 = munch1Queue;
	munch1_tuple->queue2 = munch2Queue;

	struct Queue_Tuple *munch2_tuple;
	munch2_tuple->queue1 = munch2Queue;
	munch2_tuple->queue2 = writerQueue;

	//will have to edit this around, readStream should return an updated char array
	int createReader = pthread_create(&reader, NULL, readStream, (void *) munch1Queue);
	threadCreateCheck(createReader);

	int createMunch1 = pthread_create(&munch1, NULL, firstMunch, (void *) munch1_tuple);
	threadCreateCheck(createMunch1);

	int createMunch2 = pthread_create(&munch2, NULL, secondMunch, (void *) munch2_tuple);
	threadCreateCheck(createMunch2);

	int createWriter = pthread_create(&writer, NULL, writeOutput, (void *) writerQueue);
	threadCreateCheck(createWriter);

//	Following code is to wait for when reads are finished, might have to rearrange them
//	pthread_join(reader, NULL);
//	pthread_join(munch1, NULL);
//	pthread_join(munch2, NULL);
//	pthread_join(writer, NULL);

	//when finished
	PrintQueueStats();
}

void *readStream(void *queue){
	struct Queue *queue = (struct *Queue) queue;
	/* psuedocode
	 *
	 * check line, if too big skip to next
	 * otherwise read from file and keep queueing up?
	 *
	 */
	char *str;
	int length;
	str = calloc(BUFFER_SIZE, sizeof(char));

	if(str == null){
		printf("Calloc failed on str");
		exit(-1);
	}

	while(fgets(str, BUFFER, stdin) != NULL){
		length = strlen(str);

		if(str[BUFFER_SIZE - 1] == '\0' && str[BUFFER_SIZE - 2] != '\n'){
			printf("That line was too long, flushing to new line");
			int nom;
			while((ch = fgetc(stdin)) != '\n' && nom != EOF); /* munches to the end */
		}
		else{
			EnqueueString(queue, str);
		}
	}
	return NULL;
}

void *firstMunch(void *queue){
    struct Queue_Tuple *tuple = (struct *queue_tuple) queue;
    while(!isEmpty(tuple->queue1)) {
        char *line = DequeueString(queue);
        char *idx = strchr(*line, ' ');
        while (idx) {
            idx = '*';
            idx = strchr(current_pos + 1, ' ');
        }
        EnqueueString(*tuple->queue2, line);
    }
   return NULL;
}

void *secondMunch(void *queue){
   struct Queue_Tuple *tuple = (struct *queue_tuple) queue;
    while(!isEmpty(tuple->queue1)) {
        char* line = DequeueString(queue);
        int i = 0;
        while(line[i]) {
            putchar(toupper(line[i]));
            i++;
        }
        EnqueueString(tuple->queue2, line);
    }
    return NULL;
}

void *writeOutput(void *queue){
	struct Queue* queue = (struct * Queue) queue;



	while(!IsEmpty(queue)){
		char *printMe = DequeueString(queue);
		printf("%s\n", printMe);
	}
	return NULL
}

void threadCreateCheck(int val){
	if(val != 0){
		printf("Failed thread creation \n");
		exit(-1);
	}
}


void mallocCheck(struct Queue *queue){
	if(queue == NULL){
		printf("Malloc failed at queue creation\n");
		exit(-1);
	}
}
