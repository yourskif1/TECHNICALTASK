#include <iostream>
#include <string>
#include "showdir.h"

int main() {
    std::string path = "c:/Users/Admin/projects/"; // Використовуємо поточний робочий каталог

    // Виклик функції ShowDirRecursive
    ShowDirRecursive(path);

    return 0;
}
