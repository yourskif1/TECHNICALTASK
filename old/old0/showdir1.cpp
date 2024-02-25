#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <string>

using namespace std;

// Функція для виведення вмісту поточного каталогу
void ShowDir(const string& path) {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile((path + "\\*").c_str(), &findFileData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            string name = findFileData.cFileName;
            if (name != "." && name != "..") {
                if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    cout << "Каталог: " << name << endl;
                } else {
                    cout << "Файл: " << name << endl;
                }
            }
        } while (FindNextFile(hFind, &findFileData) != 0);
        FindClose(hFind);
    }
}

int main() {
    // Отримання поточного каталогу
    char path[MAX_PATH];
    GetCurrentDirectory(sizeof(path), path);
    cout << "Поточний каталог: " << path << endl;

    // Виведення вмісту поточного каталогу
    ShowDir(path);

    return 0;
}
