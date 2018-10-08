#include <semaphore.h>

struct Queue {
    unsigned int head, foot, size;
    unsigned int capacity;
    unsigned int enqueueCount;
    unsigned int dequeueCount;
    unsigned int enqueueBlockCount;
    unsigned int dequeueBlockCount;
    sem_t mutex;
    sem_t headLock; 
    sem_t tailLock;
    char **lines;
};


Queue *CreateStringQueue(unsigned int capacity);
void EnqueueString(Queue *queue, char *string);
char * DequeueString(Queue *queue);
void PrintQueueStats(Queue *queue);
int IsFull(Queue* queue);
int IsEmpty(Queue* queue);
