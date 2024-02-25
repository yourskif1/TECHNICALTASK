CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -pedantic -pthread
LIB_NAME = libfindfile.a

all: main $(LIB_NAME)

main: main.o $(LIB_NAME)
	$(CC) $(CFLAGS) -o main main.o -L. -lfindfile

main.o: main.cpp findfile.h
	$(CC) $(CFLAGS) -c main.cpp

$(LIB_NAME): findfile.o
	ar rcs $(LIB_NAME) findfile.o

findfile.o: findfile.cpp findfile.h
	$(CC) $(CFLAGS) -c findfile.cpp

clean:
	rm -f main *.o $(LIB_NAME)
