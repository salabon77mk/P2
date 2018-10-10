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


struct Queue *CreateStringQueue(unsigned int capacity);
void EnqueueString(struct Queue *queue, char *string);
char * DequeueString(struct Queue *queue);
void PrintQueueStats(struct Queue *queue);
int IsFull(struct Queue *queue);
int IsEmpty(struct Queue *queue);
