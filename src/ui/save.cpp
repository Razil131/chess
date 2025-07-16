#include "save.hpp"
namespace fs = std::filesystem;

std::string getSaveFileName(int mode, int player, figure::teams color) {
    std::time_t now = std::time(nullptr);
    char timeStr[20];
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d-%H-%M", std::localtime(&now));
    
    std::string variantStr = (mode == 1 ? "Classic" : (mode == 2 ? "Fisher" : "3Check"));
    std::string opponentStr = player == 1 ? "Computer" : "Human";
    std::string colorStr = color == figure::WHITE ? "White" : "Black";
    
    return std::string(timeStr) + "_" + variantStr + "_" + opponentStr + "_" + colorStr;
}

bool savesExists(){

    fs::path saveDir = "../saves";

    // Проверка: существует ли папка
    if (!fs::exists(saveDir)) {
        return false;
        //"Папка не существует\n";
    }

    // Проверка: это точно папка?
    if (!fs::is_directory(saveDir)) {
        return false;
        //"Это не папка\n";
    }

    // Проверка: есть ли в ней хотя бы один файл
    for (const auto& entry : fs::directory_iterator(saveDir)) {
        if (fs::is_regular_file(entry)) {
            return true;
            break;
        }
    }
}

std::vector<std::string> getSaveFiles(const std::string& folder) {
    std::vector<std::string> saves;
    if (!savesExists())
        return saves;
    for (auto& entry : fs::directory_iterator(folder)) {
        if (entry.is_regular_file())
            saves.push_back(entry.path().filename().string());
    }
    std::sort(saves.begin(), saves.end());
    return saves;
}