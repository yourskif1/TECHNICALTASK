main: main.o libfindfile.a
	g++ -std=c++11 -Wall -Wextra -pedantic -pthread -o main main.o -L. -lfindfile

libfindfile.a: findfile.o
	ar rcs libfindfile.a findfile.o

main.o: main.cpp findfile.h
	g++ -std=c++11 -Wall -Wextra -pedantic -pthread -c main.cpp

findfile.o: findfile.cpp findfile.h
	g++ -std=c++11 -Wall -Wextra -pedantic -pthread -c findfile.cpp

clean:
	rm -f main main.o libfindfile.a findfile.o
