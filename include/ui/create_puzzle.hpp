#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <iostream>
#include <vector>
#include "board.hpp"
#include "menu.hpp"
#include "figure.hpp"
#include "modes.hpp"


void createChooseFigureMenuCreatePuzzle(sf::RenderWindow& window,
    std::map<std::string, sf::Texture>& textures,
    std::pair<int,int> selectedCell,
    int cellSize, int offsetX, int offsetY,
    sf::RectangleShape& outContainer,
    std::vector<sf::Sprite>& outSprites,
    std::vector<sf::RectangleShape>& outBackgrounds);


// отрисовать меню выбора
void drawChooseFigureMenuCreatePuzzle(sf::RenderWindow& window,
    sf::RectangleShape& container,
    std::vector<sf::Sprite>& sprites,
    std::vector<sf::RectangleShape>& backgrounds);

void drawNextModeButton(sf::RenderWindow& window, sf::RectangleShape& nextModeBtnShape, sf::Font& font, int cellSize, int offsetX, int offsetY);