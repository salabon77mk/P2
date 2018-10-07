#include <semaphore.h>

struct Queue{
  unsigned int enqueueCount;
  unsigned int dequeueCount;
  unsigned int enqueueBlockCount;
  unsigned int dequeueBlockCount;
  unsigned int queueState;
  sem_t mutex;
  sem_t ready; //don't know if needed
  char * strings;
};


Queue *CreateStringQueue(int size);
void EnqueueString(Queue *q, char *string);
char * DequeueString(Queue *q);
void PrintQueueStats(Queue *q);
