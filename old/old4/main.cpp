#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <vector>
#include <mutex>
#include "showdir.h"
#include "windows.h"

std::atomic<bool> fileFound(false);
std::mutex printMutex;

void searchThread(const std::string& directory, const std::string& filename, int threadNumber) {
    std::string result;
    while (!fileFound) {
        // Search for the file
        result = recursive_search(directory, filename);
        if (!result.empty()) {
            // If the file is found, print it and notify other threads
            {
                std::lock_guard<std::mutex> lock(printMutex);
                if (!fileFound) {
                    std::cout << "Thread " << threadNumber << " found the file: " << result << std::endl;
                    fileFound = true;
                }
            }
            break; // Exit the loop if the file is found
        }
        std::this_thread::yield();
    }
}

int main() {
    std::string directory_to_search;
    std::string searched_filename;

    std::cout << "Enter the directory path to search (or press Enter for current directory): ";
    std::getline(std::cin, directory_to_search);
    if (directory_to_search.empty()) {
        // If no directory provided, use current directory
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

    // Launch 8 search threads
    std::vector<std::thread> threads;
    for (int i = 0; i < 8; ++i) {
        threads.emplace_back(searchThread, directory_to_search, searched_filename, i + 1);
        std::cout << "Thread " << i + 1 << " started." << std::endl;
    }

    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
