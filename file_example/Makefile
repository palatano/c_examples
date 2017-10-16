CC = gcc

CFLAGS = -g -Wall

default: file_example

file_example: file_example.o
	$(CC) $(CFLAGS) -o file_example file_example.o

file_example.o: file_example.c
	$(CC) $(CFLAGS) -c file_example.c

.PHONY: clean
clean:
	rm file_example *.o *~
