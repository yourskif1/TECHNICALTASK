#include <iostream>
#include <string>
#include "findfile.h"

int main() {
    std::string startDir, targetFile;

    // Запит введення даних від користувача
    std::cout << "Enter start directory: ";
    std::getline(std::cin, startDir);

    std::cout << "Enter target file name: ";
    std::getline(std::cin, targetFile);

    // Виклик функції пошуку файлів
    findFile(startDir, targetFile);

    return 0;
}
