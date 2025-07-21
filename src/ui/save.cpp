#include "save.hpp"
namespace fs = std::filesystem;

std::string getSaveFileName(int mode, int player) { 
    std::time_t now = std::time(nullptr); //  получаем время
    char timeStr[20];
    std::strftime(timeStr, sizeof(timeStr), "%d-%m-%Y-%H-%M", std::localtime(&now)); // создаем строку день-месяц-год-час-минута
    
    std::string variantStr = (mode == 1 ? "Classic" : (mode == 2 ? "Fisher" : "3Check")); // создаем название
    std::string opponentStr = player == 1 ? "vsComputer" : "vsPlayer";
    
    return std::string(timeStr) + "_" + variantStr + "_" + opponentStr;
}

bool savesExists(){

    fs::path saveDir = "../saves"; // папка сохранений относительно 

    if (!fs::exists(saveDir)) {
        return false; // папка не существует
    }

    if (!fs::is_directory(saveDir)) {
        return false; // это не папка
    }

    for (const auto& entry : fs::directory_iterator(saveDir)) {
        if (fs::is_regular_file(entry)) {
            return true; // в ней есть файл
        }
    }
    return false;
}

std::vector<std::string> getSaveFiles(const std::string& folder) {
    std::vector<std::string> saves;
    if (!savesExists()) // если нету сохранений пустой вектор
        return saves;
    for (auto& entry : fs::directory_iterator(folder)) { 
        if (entry.is_regular_file()) // записываем все файлы
            saves.push_back(entry.path().filename().string());
    }
    std::sort(saves.begin(), saves.end()); // сортируем
    return saves;
}

std::vector<std::string> getSaveFilesWithoutDotFen(const std::string& folder) {
    std::vector<std::string> saves;
    if (!savesExists()) // если нету сохранений пустой вектор
        return saves;
    for (auto& entry : fs::directory_iterator(folder)) { 
        if (entry.is_regular_file()) // записываем все файлы
            saves.push_back(entry.path().stem().string()); // stem()-просто название файла без расширения
    }
    std::sort(saves.begin(), saves.end()); // сортируем
    return saves;
}