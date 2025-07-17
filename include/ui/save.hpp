#pragma once
#include <string>
#include <filesystem>
#include "figure.hpp"

std::string getSaveFileName(int mode, int player); // получить строку с названием файла который должен быть у сохранения
bool savesExists(); // есть ли вообще какие то сохранения
std::vector<std::string> getSaveFiles(const std::string& folder = "../saves"); // получить все названия сохранений
std::vector<std::string> getSaveFilesWithoutDotFen(const std::string& folder = "../saves"); // получить все названия сохранений без .fen