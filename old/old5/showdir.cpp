// showdir.cpp
#include "showdir.h"
#include <iostream>
#include <windows.h>

std::string recursive_search(const std::string& directory, const std::string& filename) {
    std::string result;

    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((directory + "\\*").c_str(), &findFileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            std::string name = findFileData.cFileName;
            if (name != "." && name != "..") {
                if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    // Рекурсивний виклик для підкаталогів
                    std::string subdir = directory + "\\" + name;
                    result = recursive_search(subdir, filename);
                    if (!result.empty()) {
                        break; // Якщо знайдено файл, завершуємо пошук
                    }
                } else {
                    if (name == filename) {
                        result = directory + "\\" + name;
                        break; // Якщо знайдено файл, завершуємо пошук
                    }
                }
            }
        } while (FindNextFile(hFind, &findFileData) != 0);
        FindClose(hFind);
    }

    return result;
}
