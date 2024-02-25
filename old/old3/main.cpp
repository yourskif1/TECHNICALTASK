#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <windows.h> // Додано
#include "showdir.h"  // Додано
#include <vector>    // Додано

std::atomic<bool> stopSearch(false);

void searchThread(const std::string& directory, const std::string& filename) {
    while (!stopSearch) {
        std::string result = recursive_search(directory, filename);
        if (!result.empty()) {
            std::cout << "File found: " << result << std::endl;
            stopSearch = true;
        }
    }
}

int main() {
    std::string directory_to_search;
    std::string searched_filename;

    std::cout << "Enter the directory path to search (or press Enter for current directory): ";
    std::getline(std::cin, directory_to_search);

    // Використовуємо текучий каталог, якщо введений шлях порожній
    if (directory_to_search.empty()) {
        char current_path[MAX_PATH];
        GetCurrentDirectory(MAX_PATH, current_path);
        directory_to_search = current_path;
    }

    std::cout << "Enter the filename to search: ";
    std::getline(std::cin, searched_filename);

    if (directory_to_search.empty() || searched_filename.empty()) {
        std::cout << "Directory path or filename cannot be empty." << std::endl;
        return 1;
    }

    std::vector<std::thread> threads;
    for (int i = 0; i < 8; ++i) {
        threads.emplace_back(searchThread, directory_to_search, searched_filename);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
