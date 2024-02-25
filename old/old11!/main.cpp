// main.cpp

#include <iostream>
#include <string>
#include "findfile.h"

int main() {
    std::string startDir;
    std::cout << "Enter start directory: ";
    std::cin >> startDir;

    std::string targetFile;
    std::cout << "Enter target file name: ";
    std::cin >> targetFile;

    pendingDirectories.push_back(startDir);
    parallelSearch(targetFile, 8);

    return 0;
}
