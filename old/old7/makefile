CC = g++
CFLAGS = -std=c++17 -Wall -Wextra -pedantic -pthread

all: findfile

findfile: findfile.cpp
	$(CC) $(CFLAGS) -o findfile findfile.cpp

clean:
	rm -f findfile
