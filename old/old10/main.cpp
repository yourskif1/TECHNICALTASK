// main.cpp

#include <iostream>
#include <string>
#include "findfile.h" // Підключення заголовного файлу findfile.h

int main() {
    std::string startDir;
    std::cout << "Enter start directory: ";
    std::cin >> startDir;

    std::string targetFile;
    std::cout << "Enter target file name: ";
    std::cin >> targetFile;

    pendingDirectories.push_back(startDir);
    parallelSearch(targetFile, 8); // Виклик функції parallelSearch

    return 0;
}
