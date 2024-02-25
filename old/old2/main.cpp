#include <iostream>
#include <windows.h>
#include "showdir.h"

int main() {
    char current_path[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, current_path);

    std::cout << "Current Directory: " << current_path << std::endl;

    std::string directory_path, filename;

    std::cout << "Enter the directory path to search (press Enter for current directory): ";
    std::getline(std::cin, directory_path);

    if (directory_path.empty()) {
        directory_path = current_path;
    }

    std::cout << "Enter the filename to search: ";
    std::getline(std::cin, filename);

    std::string result = recursive_search(directory_path, filename);
    if (!result.empty()) {
        std::cout << "File found: " << result << std::endl;
    } else {
        std::cout << "No file found" << std::endl;
    }

    return 0;
}
