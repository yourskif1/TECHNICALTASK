#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <deque>
#include <cstring>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <vector>
#include <sys/stat.h>

std::mutex mtx;
std::deque<std::string> directoriesToSearch;
std::string searched_filename;

void enqueue_directory(const std::string& directory) {
    std::lock_guard<std::mutex> lock(mtx);
    directoriesToSearch.push_back(directory);
}

std::string dequeue_directory() {
    std::lock_guard<std::mutex> lock(mtx);
    if (directoriesToSearch.empty()) {
        return "";
    }
    std::string directory = directoriesToSearch.front();
    directoriesToSearch.pop_front();
    return directory;
}

std::string get_next_directory_to_search() {
    while (true) {
        std::string directory = dequeue_directory();
        if (directory.empty()) {
            return "";
        }
        return directory;
    }
}

void searchThread(int threadNumber) {
    std::string currentDirectory;
    while (true) {
        currentDirectory = get_next_directory_to_search();
        if (currentDirectory.empty()) {
            break;
        }
        std::cout << "Thread " << threadNumber << " is searching in directory: " << currentDirectory << std::endl;

        DIR* dir;
        struct dirent* dp;

        if ((dir = opendir(currentDirectory.c_str())) == NULL) {
            continue;
        }

        while ((dp = readdir(dir)) != NULL) {
            if (std::strcmp(dp->d_name, ".") != 0 && std::strcmp(dp->d_name, "..") != 0) {
                std::string path = currentDirectory + "/" + dp->d_name;

                struct stat st;
                if (stat(path.c_str(), &st) == -1) {
                    continue;
                }

                if (S_ISDIR(st.st_mode)) {
                    std::string subdir_path = currentDirectory + "/" + dp->d_name;
                    enqueue_directory(subdir_path);
                } else {
                    if (searched_filename.empty() || searched_filename == dp->d_name) {
                        std::cout << "Thread " << threadNumber << " found file: " << path << std::endl;
                    }
                }
            }
        }
        closedir(dir);
    }
}

int main() {
    std::string directory_to_search;
    std::cout << "Enter the directory path to search (or press Enter for current directory): ";
    std::getline(std::cin, directory_to_search);
    if (directory_to_search.empty()) {
        char current_path[FILENAME_MAX];
        if (getcwd(current_path, sizeof(current_path)) != NULL) {
            directory_to_search = current_path;
        } else {
            std::cerr << "Error: Unable to get current directory." << std::endl;
            return 1;
        }
    }

    std::cout << "Enter the filename to search (or press Enter for listing directories): ";
    std::getline(std::cin, searched_filename);

    enqueue_directory(directory_to_search);

    std::deque<std::thread> threads;
    for (int i = 1; i <= 8; ++i) {
        std::cout << "Thread " << i << " is starting..." << std::endl;
        threads.emplace_back(searchThread, i);
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return 0;
}
