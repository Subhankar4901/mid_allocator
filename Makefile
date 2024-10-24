CC=gcc
CFLAGS=-Wall -ggdb -Wno-frame-address
all:run
#default
run:main allocator utils
	$(CC) $(CFLAGS) -o run main.o allocator.o utils.o
main:main.c
	$(CC) $(CFLAGS) -c main.c -o main.o
allocator:allocator/allocator.c
	$(CC) $(CFLAGS) -c allocator/allocator.c -o allocator.o
utils: utils/list_utils.c utils/logging.c
	$(CC) $(CFLAGS) -c utils/list_utils.c -o list_utils.o
	$(CC) $(CFLAGS) -c utils/logging.c -o logging.o
	$(CC) $(CFLAGS) -r list_utils.o logging.o -o utils.o
#tests
test: test.o allocator utils
	$(CC) $(CFLAGS) -o test test.o allocator.o utils.o
test.o:
	$(CC) $(CFLAGS) -c tests/test.c -o test.o
clean:
	rm -f *.o run test