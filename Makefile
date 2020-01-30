CC=gcc
CFLAGS=-Wall

sorting_threads: sorting_threads.o
	$(CC) $(CFLAGS) -o sorting_threads sorting_threads.o -lpthread

sorting_threads.o: sorting_threads.c
	$(CC) $(CFLAGS) -c sorting_threads.c -lpthread

clean:
	rm -rf *.o