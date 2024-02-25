#include "findfile.h"
#include <iostream>
#include <dirent.h>
#include <sys/stat.h>

void findFile(const std::string& directory, const std::string& target) {
    DIR* dir = opendir(directory.c_str());
    if (!dir) {
        std::cerr << "Error opening directory: " << directory << std::endl;
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        std::string filename = entry->d_name;
        if (filename != "." && filename != "..") {
            std::string filepath = directory + "/" + filename;
            struct stat fileInfo;
            if (stat(filepath.c_str(), &fileInfo) != 0) {
                std::cerr << "Error getting file info: " << filepath << std::endl;
                continue;
            }

            if (S_ISDIR(fileInfo.st_mode)) {
                // Рекурсивно шукати у підкаталогах
                findFile(filepath, target);
            } else if (filename == target) {
                std::cout << "File found: " << filepath << std::endl;
            }
        }
    }
    closedir(dir);
}
