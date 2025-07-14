#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "board.hpp"
#include "figure.hpp"

void drawFigures(sf::RenderWindow& window, Board* board,  float CELLSIZE, float OFFSETX, float OFFSETY);  // нарисовать фигуры


void drawMoveHighlights( // нарисовать все возможные ходы
    sf::RenderWindow& window,
    const std::vector<std::pair<int, int>>& possibleMoves,
    const Board& board,
    figure* selectedFigure,
    float OFFSETX,
    float OFFSETY,
    float CELLSIZE
);


void drawCheck(sf::RenderWindow& window, // отрисовать красный квадрат на короле с шахом
    Board* board,
    figure::teams team,
    float OFFSETX,
    float OFFSETY,
    float CELLSIZE);

