CC = g++
CFLAGS = -Wall -std=c++11

LIB_NAME = libshowdir.a
SRC_DIR = .
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

.PHONY: all clean

all: $(LIB_NAME) main

$(LIB_NAME): $(OBJS)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

main: main.cpp $(LIB_NAME)
	$(CC) $(CFLAGS) main.cpp -o main -L. -lshowdir

clean:
	rm -rf $(OBJ_DIR) $(LIB_NAME) main
