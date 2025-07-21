#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <vector>
#include "board.hpp"
#include "figure.hpp"

// создать меню выбора 
void createChoiceMenu(const Board* board,
    std::vector<sf::Sprite>& to_choose,
    std::vector<sf::RectangleShape>& rectanges_to_choose,
    std::map<std::string,sf::Texture>& textures,
    float OFFSETX,
    float OFFSETY,
    float CELLSIZE);

// отрисовать меню выбора
void drawChoiceMenu(sf::RenderWindow& window,std::vector<sf::Sprite>& to_choose, std::vector<sf::RectangleShape> rectangles_to_choose);

// кликнуть по фигуре в меню выбора
void selectFigureToConvert(Board* board,
    const std::vector<sf::RectangleShape> rectangles_to_choose,
    const sf::Vector2f& mousePos,
    std::map<std::string, sf::Texture>& textures,
    float OFFSETX,
    float CELLSIZE);