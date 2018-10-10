CC=gcc
CFLAGS=-g -Wall -Wextra -pthread
DEBUG=-g
DEPS = queue.h
OBJ = main.o queue.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

prodcomm: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean
	
clean:
		rm *.o prodcomm

.PHONY: debug

debug: $(OBJ)
	$(CC) -o -g $@ $^ $(CFLAGS)
