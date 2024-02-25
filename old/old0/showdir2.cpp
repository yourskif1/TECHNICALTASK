#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <string>

using namespace std;

// Функція для виведення вмісту каталогу та його підкаталогів
void ShowDirRecursive(const string& path) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((path + "\\*").c_str(), &findFileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            string name = findFileData.cFileName;
            if (name != "." && name != "..") {
                if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    cout << "Directory: " << name << endl;
                    // Рекурсивний виклик для підкаталогів
                    ShowDirRecursive(path + "\\" + name);
                } else {
                    cout << "File: " << name << endl;
                }
            }
        } while (FindNextFile(hFind, &findFileData) != 0);
        FindClose(hFind);
    }
}

int main() {
    char path[MAX_PATH];
    GetCurrentDirectory(sizeof(path), path);
    cout << "Current Directory: " << path << endl;

    string newPath;
    cout << "Enter new directory path (or press Enter to keep current): ";
    getline(cin, newPath);
    if (!newPath.empty()) {
        if (newPath.substr(0, 3) == "c:\\") {
            SetCurrentDirectory(newPath.c_str());
            cout << "Directory changed to: " << newPath << endl;
        } else {
            cout << "Invalid directory path. Directory not changed." << endl;
        }
    }

    string fileName;
    cout << "Enter file name to search (or press Enter to skip): ";
    getline(cin, fileName);
    if (!fileName.empty()) {
        cout << "Searching for file: " << fileName << endl;
        // Додайте код для пошуку файлу в поточному каталозі та його підкаталогах
    }

    // Виведення вмісту поточного каталогу та його підкаталогів
    ShowDirRecursive(path);

    return 0;
}

