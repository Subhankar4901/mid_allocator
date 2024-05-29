CC=gcc
CFLAGS=-Wall -g
all:run
run:main utills debug 
	$(CC) $(CFLAGS) -o run main.o list_utils.o debug.o
main:main.c
	$(CC) $(CFLAGS) -c main.c -o main.o
debug:debug/debug.c
	$(CC) $(CFLAGS) -c debug/debug.c -o debug.o
utills: utils/list_utils.c
	$(CC) $(CFLAGS) -c utils/list_utils.c -o list_utils.o
clean:
	rm -f *.o main