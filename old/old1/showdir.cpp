#include "showdir.h"
#include <iostream>
#include <windows.h>

void ShowDirRecursive(const std::string& path) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((path + "\\*").c_str(), &findFileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            std::string name = findFileData.cFileName;
            if (name != "." && name != "..") {
                if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    std::cout << "Directory: " << name << std::endl;
                    // Рекурсивний виклик для підкаталогів
                    ShowDirRecursive(path + "\\" + name);
                } else {
                    std::cout << "File: " << name << std::endl;
                }
            }
        } while (FindNextFile(hFind, &findFileData) != 0);
        FindClose(hFind);
    }
}
