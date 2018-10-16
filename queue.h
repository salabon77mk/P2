/* AUtHORS
 * Mykola Kusyy
 * Garrett McLaughlin
 */
#include <pthread.h>


struct Queue{
    unsigned int head, tail, size;
    unsigned int capacity;
    unsigned int enqueueCount;
    unsigned int dequeueCount;
    unsigned int enqueueBlockCount;
    unsigned int dequeueBlockCount;
    pthread_mutex_t mutex;
    pthread_cond_t headLock; 
    pthread_cond_t tailLock;
    char **lines;
};


struct Queue *CreateStringQueue(unsigned int capacity);
void EnqueueString(struct Queue *queue, char *string);
char * DequeueString(struct Queue *queue);
void PrintQueueStats(struct Queue *queue);

