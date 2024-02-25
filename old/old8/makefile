CC = g++
CFLAGS = -std=c++11 -Wall -Wextra -pedantic -pthread
LIB_NAME = libfindfile.a

all: main $(LIB_NAME)

main: main.o findfile.o
	$(CC) $(CFLAGS) -o main main.o findfile.o

main.o: main.cpp findfile.h
	$(CC) $(CFLAGS) -c main.cpp

findfile.o: findfile.cpp findfile.h
	$(CC) $(CFLAGS) -c findfile.cpp

$(LIB_NAME): findfile.o
	ar rcs $(LIB_NAME) findfile.o

clean:
	rm -f main *.o $(LIB_NAME)
