#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic> // Додано це включення
#include "findfile.h"

std::mutex mtx;
std::condition_variable cv;
std::vector<std::string> pendingDirectories;
std::vector<std::string> foundFiles;
std::atomic<int> remainingThreads;
std::atomic<bool> fileFound(false);

int main() {
    std::string startDir, targetFile;

    std::cout << "Enter start directory: ";
    std::getline(std::cin, startDir);
    if (startDir.empty()) startDir = ".";

    std::cout << "Enter target file name: ";
    std::getline(std::cin, targetFile);

    if (targetFile.empty()) {
        std::cerr << "No target file specified." << std::endl;
        return 1;
    }

    pendingDirectories.push_back(startDir);
    remainingThreads = 8;

    std::vector<std::thread> threads;
    for (int i = 0; i < 8; ++i) {
        threads.emplace_back(findFile, targetFile);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "\nFound files:" << std::endl;
    for (const auto& file : foundFiles) {
        std::cout << file << std::endl;
    }

    return 0;
}
