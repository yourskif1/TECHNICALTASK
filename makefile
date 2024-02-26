SRC_DIR = c:\Users\Admin\projects\TechnicalTask
OBJ_DIR = $(SRC_DIR)\obj
BIN_DIR = $(SRC_DIR)\bin
LIB_DIR = $(SRC_DIR)\lib

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -pthread

# Список вихідних файлів без розширення
EXECUTABLE = $(BIN_DIR)\main
LIBRARY = $(LIB_DIR)\libfindfile.a

# Список вихідних файлів з розширеннями
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Основна ціль: компіляція виконуваного файлу та бібліотеки
all: directories $(EXECUTABLE) $(LIBRARY)

# Створення каталогів, якщо вони не існують
directories:
	mkdir -p $(OBJ_DIR) $(BIN_DIR) $(LIB_DIR)

# Компіляція виконуваного файлу
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Компіляція об'єктних файлів
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Створення статичної бібліотеки
$(LIBRARY): $(OBJECTS)
	ar rcs $@ $^

# Видалення об'єктних файлів, виконуваного файлу та бібліотеки
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) $(LIB_DIR)