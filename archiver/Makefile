CC = gcc

CFLAGS = -g -Wall

default: archiver

archiver: archiver.o archive_func.o archive_info.o archive_size.o
	$(CC) $(CFLAGS) -o archiver archiver.o archive_func.o archive_info.o archive_size.o

archiver.o: archiver.c
	$(CC) $(CFLAGS) -c archiver.c

archive_func.o: archive_func.c archive_func.h
	$(CC) $(CFLAGS) -c archive_func.c

archive_info.o: archive_info.c archive_info.h
	$(CC) $(CFLAGS) -c archive_info.c

archive_size.o: archive_size.c archive_size.h archive_constants.h
	$(CC) $(CFLAGS) -c archive_size.c

.PHONY: clean
clean: 
	rm archiver *.o
