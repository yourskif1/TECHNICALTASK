// findfile.cpp

#include "findfile.h"
#include <iostream>
#include <string>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
#include <thread>
#include <mutex>

std::mutex mtx;
bool stopSearch = false;
std::vector<std::string> pendingDirectories; // using vector and mutex for searching file without recursive function

void findFile(const std::string& target) {
    while (true) {
        std::string currentDir;
        {
            std::lock_guard<std::mutex> lock(mtx);
            if (stopSearch || pendingDirectories.empty()) return;
            currentDir = pendingDirectories.back();
            pendingDirectories.pop_back();
        }

        DIR* dir = opendir(currentDir.c_str());
        if (!dir) {
            std::cerr << "Error opening directory: " << currentDir << std::endl;
            continue;
        }

//        std::cout << "Thread " << std::this_thread::get_id() << " processing directory: " << currentDir << std::endl;

        struct dirent* entry;
        while ((entry = readdir(dir)) != nullptr) {
            std::string filename = entry->d_name;
            if (filename == "." || filename == "..") {
                continue;
            }

            std::string filepath = currentDir + "/" + filename;
            struct stat fileInfo;
            if (stat(filepath.c_str(), &fileInfo) != 0) {
                std::cerr << "Error getting file info: " << filepath << std::endl;
                continue;
            }

            if (S_ISDIR(fileInfo.st_mode)) {
                std::lock_guard<std::mutex> lock(mtx);
                pendingDirectories.push_back(filepath);
            } else if (S_ISREG(fileInfo.st_mode) && filename == target) {
                std::lock_guard<std::mutex> lock(mtx);
                stopSearch = true;
                std::cout << "File found by thread " << std::this_thread::get_id() << ": " << filepath << std::endl;
                return;
            }
        }
        closedir(dir);
    }
}

void parallelSearch(const std::string& target, int numThreads) {
    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(findFile, std::ref(target));
        std::cout << "Starting thread " << threads[i].get_id() << "..." << std::endl;
    }

    for (auto& thread : threads) {
        thread.join();
    }
}