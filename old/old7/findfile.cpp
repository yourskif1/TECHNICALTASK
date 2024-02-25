#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <string>
#include <filesystem>
#include <condition_variable>
#include <atomic>

namespace fs = std::filesystem;

std::mutex mtx;
std::condition_variable cv;
std::vector<std::string> pendingDirectories;
std::vector<std::pair<std::thread::id, std::string>> foundFiles;
std::atomic<int> remainingThreads;
std::atomic<bool> fileFound(false); // Флаг, що позначає, чи був знайдений файл

void findFile(const std::string& target) {
    while (true) {
        std::string currentDir;
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, []{ return !pendingDirectories.empty(); });
            if (pendingDirectories.empty()) {
                // Зменшення лічильника залишкових потоків і виходячи із циклу
                remainingThreads--;
                return;
            }
            currentDir = pendingDirectories.back();
            pendingDirectories.pop_back();
        }

//        std::cout << "Thread " << std::this_thread::get_id() << " started in directory: " << currentDir << std::endl;

        try {
            for (const auto& entry : fs::directory_iterator(currentDir)) {
                const auto& filepath = entry.path().string();
                if (fs::is_directory(entry.status())) {
                    {
                        std::lock_guard<std::mutex> lock(mtx);
                        pendingDirectories.push_back(filepath);
                    }
                    cv.notify_one(); // Сповіщення інших потоків про наявність нового каталогу
                } else if (fs::is_regular_file(entry.status()) && entry.path().filename().compare(target) == 0) {
                    std::lock_guard<std::mutex> lock(mtx);
                    if (!fileFound.exchange(true)) { // Встановлення флагу, якщо він ще не був встановлений
                        std::cout << "Thread " << std::this_thread::get_id() << " found target file: " << filepath << std::endl;
                        foundFiles.emplace_back(std::this_thread::get_id(), filepath);
                        cv.notify_all(); // Повідомлення інших потоків, щоб вони завершили свою роботу
                    }
                    // Якщо файл знайдено, інші потоки більше не потрібні, тому вони можуть вийти з функції
                    return;
                }
                // Якщо файл знайдено іншим потоком, цей потік теж має завершити свою роботу
                if (fileFound.load())
                    return;
            }
        } catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

int main() {
    std::string startDir, targetFile;
    
    // Запит введення даних від користувача
    std::cout << "Enter start directory: ";
    std::getline(std::cin, startDir);
    if (startDir.empty()) startDir = "."; // Використовувати поточний каталог, якщо ввід порожній

    std::cout << "Enter target file name: ";
    std::getline(std::cin, targetFile);

    // Використовувати поточний каталог, якщо ввід порожній
    if (targetFile.empty()) {
        std::cerr << "No target file specified." << std::endl;
        return 1;
    }

    pendingDirectories.push_back(startDir);
    remainingThreads = 8; // Ініціалізація лічильника залишкових потоків

    // Створення вектора для зберігання потоків
    std::vector<std::thread> threads;

    // Створення 8 потоків
    for (int i = 0; i < 8; ++i) {
        threads.emplace_back(findFile, targetFile);
    }

    // Чекаємо завершення всіх потоків або завершення обробки всіх каталогів
    for (auto& thread : threads) {
        thread.join();
    }

    // Виведення знайдених файлів після завершення
    std::cout << "\nFound files:" << std::endl;
    for (const auto& pair : foundFiles) {
        std::cout << "Thread " << pair.first << " found target file: " << pair.second << std::endl;
    }

    return 0;
}
