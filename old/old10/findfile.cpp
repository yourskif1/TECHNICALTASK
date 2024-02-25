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
std::vector<std::string> pendingDirectories; // Оголошення змінної pendingDirectories

bool stopSearch = false;

void findFile(const std::string& target) {
    while (true) {
        std::string currentDir;
        {
            std::lock_guard<std::mutex> lock(mtx);
            if (stopSearch) return;
            // отримуємо наступний каталог для пошуку
            if (pendingDirectories.empty()) return;
            currentDir = pendingDirectories.back();
            pendingDirectories.pop_back();
        }

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
            } else if (S_ISREG(fileInfo.st_mode) && filename == target) {
                std::lock_guard<std::mutex> lock(mtx);
                std::cout << "File found by thread " << std::this_thread::get_id() << ": " << filepath << std::endl;
                stopSearch = true; // зупиняємо пошук
                return;
            }
        }
        closedir(dir);
    }
}

void parallelSearch(const std::string& target, int numThreads) {
    std::vector<std::thread> threads;
    for (int i = 0; i < numThreads; ++i) {
        std::cout << "Starting thread " << i + 1 << "..." << std::endl; // Вивід, коли потік розпочинає роботу
        threads.emplace_back(findFile, target);
    }

    for (auto& thread : threads) {
        thread.join();
    }
}
