struct Queue{
  unsigned int enqueueCount;
  unsigned int dequeueCount;
  unsigned int enqueueBlockCount;
  unsigned int dequeueBlockCount;
  char * strings;
};


Queue *CreateStringQueue(int size);
void EnqueueString(Queue *q, char *string);
char * DequeueString(Queue *q);
void PrintQueueStats(Queue *q);
