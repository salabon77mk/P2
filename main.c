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

int main(void){

	pthread_t reader, munch1, munch2, writer;
	struct Queue *munch1Queue = malloc(sizeof(struct *Queue));
	mallocCheck(munch1Queue);

	struct Queue *munch2Queue = malloc(sizeof(struct *Queue));
	mallocCheck(munch2Queue);

	struct Queue *writerQueue = malloc(sizeof(struct *Queue));
	mallocCheck(writerQueue);

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

	while(fgets(str, BUFFER, stdin) != NULL){
		length = strlen(str);

		if(str[BUFFER_SIZE - 1] == '\0' && str[BUFFER_SIZE - 2] != '\n'){
			printf("That line was too long, flushing to new line");
			int nom;
			while((ch = fgetc(stdin)) != '\n'){/* munches to the end */}
		}
		else{
			EnqueueString(queue, str);
		}
	}
	return (void *) queue;
}

void *firstMunch(void *queue){
    while(!isEmpty(/* MUNCH1 QUEUE */)) {
        char* line = DequeueString(queue);
        char *idx = strchr(*line, ' ');
        while (idx) {
            *idx = '*';
            idx = strchr(current_pos+1, ' ');
        }
        EnqueueString(/* MUNCH2 QUEUE */, line)
    }
}

void *secondMunch(void *queue){
    while(!isEmpty(/* MUNCH2 QUEUE */)) {
        char* line = DequeueString(queue);
        int i = 0;
        while(*(line[i])) {
            putchar(toupper(*(line[i])));
            i++;
        }
        EnqueueString(/* WRITER QUEUE */)
    }
}

void *writeOutput(void *queue){
	struct Queue* queue = (struct * Queue) queue;
	while(!IsEmpty(queue)){
		char *printMe = DequeueString(queue);
		printf("%s\n", printMe);
	}
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
