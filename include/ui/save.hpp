#pragma once
#include <string>
#include <filesystem>
#include "figure.hpp"

std::string getSaveFileName(int mode, int player, figure::teams color);
bool savesExists();
std::vector<std::string> getSaveFiles(const std::string& folder = "../saves");