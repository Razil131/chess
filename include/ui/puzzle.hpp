#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <iostream>
#include <vector>
#include <filesystem>
#include <set>
#include "board.hpp"
#include "menu.hpp"
#include "figure.hpp"
#include "modes.hpp"
namespace fs = std::filesystem;

// создать меню выбора фигуры для постановки ее на доску
void createChooseFigureMenuCreatePuzzle(sf::RenderWindow& window,
    std::map<std::string, sf::Texture>& textures,
    std::pair<int,int> selectedCell,
    int cellSize, int offsetX, int offsetY,
    sf::RectangleShape& outContainer,
    std::vector<sf::Sprite>& outSprites,
    std::vector<sf::RectangleShape>& outBackgrounds);


// отрисовать меню выбора фигуры для постановки ее на доску
void drawChooseFigureMenuCreatePuzzle(sf::RenderWindow& window,
    sf::RectangleShape& container,
    std::vector<sf::Sprite>& sprites,
    std::vector<sf::RectangleShape>& backgrounds);
    
// отрисовать кнопку продолжения перехода в следующий режим
void drawNextModeButton(sf::RenderWindow& window, sf::RectangleShape& nextModeBtnShape, sf::Font& font, int cellSize, int offsetX, int offsetY);

// вернуть количество файлов в папке puzzles (задач)
int countPuzzles(const std::string& folder = "../puzzles"); 

// вернуть название нового пазла
std::string generatePuzzleFilename(const std::string& folder = "../puzzles");

// вернуть название пазла по его номеру
std::string makePuzzleFilename(std::string& numberStr); 

// удалить пазл по его номеру
bool deletePuzzleByNumber(const std::string& numberStr, const std::string& folder = "../puzzles/");

std::vector<int> getPuzzleNumbers(const std::string& folder = "../puzzles");