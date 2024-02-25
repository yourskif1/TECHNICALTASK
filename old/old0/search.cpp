#include "search.h"

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>

std::string recursive_search(const std::string& directory, const std::string& filename) {
    DIR *dir;
    struct dirent *entry;
    if ((dir = opendir(directory.c_str())) != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            std::string current_file(entry->d_name);
            if (current_file == filename) {
                closedir(dir);
                return directory + "/" + current_file;
            }
            std::string full_path = directory + "/" + current_file;
            struct stat statbuf;
            if (stat(full_path.c_str(), &statbuf) == -1) {
                continue;
            }
            if (S_ISDIR(statbuf.st_mode) && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
                std::string subdir = directory + "/" + current_file;
                std::string found_file = recursive_search(subdir, filename);
                if (!found_file.empty()) {
                    closedir(dir);
                    return found_file;
                }
            }
        }
        closedir(dir);
    } else {
        std::cerr << "Error opening directory: " << directory << std::endl;
    }
    return ""; // File not found
}
