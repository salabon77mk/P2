CC=gcc
CFLAGS=-g -Wall -Wextra -lpthread
DEPS = queue.h
OBJ = main.o queue.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

prodcomm: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
	
clean:
		rm *.o prodcomm

