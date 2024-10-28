CC = gcc
CFLAGS = -Wall -ggdb -Wno-frame-address
PROD_FLAGS= -Wall -O3 -DNDEBUG -march=native -Wno-frame-address
#Tests
# make or just make test
TEST_DIR = tests
UTILS_SRC = utils/list_utils.c utils/logging.c
TEST_SRC := $(wildcard $(TEST_DIR)/*.c)
TEST_OBJS := $(patsubst %.c,%.o,$(TEST_SRC))
TEST_TARGET = test
all: test
allocator.o: allocator/allocator.c
	$(CC) $(CFLAGS) -c $< -o $@
utils.o: list_utils.o logging.o
	$(CC) $(CFLAGS) -r $^ -o $@
list_utils.o: utils/list_utils.c
	$(CC) $(CFLAGS) -c $< -o $@
logging.o: utils/logging.c
	$(CC) $(CFLAGS) -c $< -o $@
$(TEST_TARGET): allocator.o utils.o $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $@ $^
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@


#Library creation
LIB_NAME=libmid_alloc.so
lib:allocator_pic.o list_utils_pic.o logging_pic.o
	$(CC) $(PROD_FLAGS) -shared -o $(LIB_NAME)  allocator_pic.o list_utils_pic.o logging_pic.o
allocator_pic.o: allocator/allocator.c utils/utils.h
	$(CC) $(PROD_FLAGS) -fPIC -c allocator/allocator.c -o allocator_pic.o
list_utils_pic.o: utils/utils.h utils/list_utils.c 
	$(CC) $(PROD_FLAGS) -fPIC -c utils/list_utils.c -o list_utils_pic.o
logging_pic.o: utils/utils.h utils/logging.c
	$(CC) $(PROD_FLAGS) -fPIC -c utils/logging.c -o logging_pic.o

#Install library
install:lib
	cp $(LIB_NAME) /usr/local/lib
	ldconfig

# Clean
clean:
	rm -f *.o run test $(TEST_DIR)/*.o *.so
