#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>
#include <memory>
#include "../include/figure.hpp"
#include "../include/bishop.hpp"
#include "../include/king.hpp"
#include "../include/knight.hpp"
#include "../include/pawn.hpp"
#include "../include/queen.hpp"
#include "../include/rook.hpp"

class figure;


void setupFigures( // расставить все фигуры по стандарту на gui
    std::map<std::string, sf::Texture>& textures, // текстуры
    std::vector<std::unique_ptr<figure>>& figures, // вектор с указателями на фигуры
    float offsetX, // отступ для букв
    float offsetY, // отступ для цифр
    float cellSize); // размер клетки
