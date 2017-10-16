CC = gcc

CFLAGS = -g  -Wall

default: mylogger

mylogger: mylogger.o log_linkedlist.o sort.o
	$(CC) $(CFLAGS) -o mylogger mylogger.o sort.o log_linkedlist.o

mylogger.o: mylogger.c
	$(CC) $(CFLAGS) -c mylogger.c

sort.o: sort.c sort.h
	$(CC) $(CFLAGS) -c sort.c

log_linkedlist.o: log_linkedlist.c log_linkedlist.h
	$(CC) $(CFLAGS) -c log_linkedlist.c

.PHONY: clean
clean:
	rm mylogger *.o *~
