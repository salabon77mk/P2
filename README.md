# P2

We made our queue synchronized using pthreads. To make our code there cleaner,
we wrote wrapper functions so we don't have error checks populating areas of 
code that should be clear and easy to read.

In main, we decided to separately name our threads instead of creating an array
of threads so each thread could easily be discerned and since the threads
have unique purposes, we figured unique variable names would improve
readability. 

Our reader thread uses getc to read in each line character by character until
we reach either a newline or NULL (end of file). This reader reserves the second
to last char in the buffer for new line and the last char for the terminating NULL
that all strings have. If the second to last number of the buffer has been reached
and there's no newline found, we reset the allocated string, and flush to the end of 
the line. Once all lines have been read, we enqueue a NULL which will start
the termination process for every successive thread.

Munch1 and munch2 were straightforward to implement. We decided a do, while loop, 
while cleaner, could be bug prone in case the very first string dequeued is NULL.

Writer was straight forward.

We created some helper functions for the purpose of return value checking in main
just as we did in queue.c
