#include "findfile.h"
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>
#include <string>
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include <thread>

namespace {
    std::mutex mtx;
    std::condition_variable cv;
    std::vector<std::string> pendingDirectories;
    std::vector<std::string> foundFiles;
    std::atomic<int> remainingThreads;
    std::atomic<bool> fileFound(false);
}

void findFile(const std::string& target) {
    while (true) {
        std::string currentDir;
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, []{ return !pendingDirectories.empty(); });
            if (pendingDirectories.empty()) {
                if (--remainingThreads == 0 && !fileFound) {
                    std::cerr << "File not found." << std::endl;
                }
                return;
            }
            currentDir = pendingDirectories.back();
            pendingDirectories.pop_back();
        }

        std::cout << "Thread " << std::this_thread::get_id() << " is processing directory: " << currentDir << std::endl;

        DIR* dir = opendir(currentDir.c_str());
        if (!dir) {
            std::cerr << "Error opening directory: " << currentDir << std::endl;
            continue;
        }

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
                cv.notify_one();
                remainingThreads++;
            } else if (S_ISREG(fileInfo.st_mode) && filename == target) {
                std::lock_guard<std::mutex> lock(mtx);
                if (!fileFound.exchange(true)) {
                    foundFiles.push_back(filepath);
                    std::cout << "Thread " << std::this_thread::get_id() << " found the file: " << filepath << std::endl;
                }
                return;
            }
        }
        closedir(dir);
    }
}
