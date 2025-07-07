#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>
#include <memory>
#include "figure.hpp"
#include "bishop.hpp"
#include "king.hpp"
#include "knight.hpp"
#include "pawn.hpp"
#include "queen.hpp"
#include "rook.hpp"

class figure;


void setupFigures( // расставить все фигуры по стандарту на gui
    std::map<std::string, sf::Texture>& textures, // текстуры
    std::vector<std::unique_ptr<figure>>& figures, // вектор с указателями на фигуры
    float offsetX, // отступ для букв
    float offsetY, // отступ для цифр
    float cellSize); // размер клетки


void initializeBoardRectangles( // расставить все клетки доски
    sf::RectangleShape boardRectangles[8][8],
    float CELLSIZE,
    float OFFSETX,
    float OFFSETY
);

bool loadFont(sf::Font& font); // загрузить шрифт

void createLetters( // создать буквы
    sf::Text letters[8],
    const sf::Font& font,
    float CELLSIZE,
    float OFFSETX,
    float OFFSETY
);

void createNumbers( // создать цифры
    sf::Text numbers[8],
    const sf::Font& font,
    float CELLSIZE,
    float OFFSETY
);

void drawBoardAndLabels( // нарисовать цифры и буквы
    sf::RenderWindow& window,
    sf::RectangleShape boardRectangles[8][8],
    const sf::Text letters[8],
    const sf::Text numbers[8]
);

void drawFigures( // нарисовать фигуры
    sf::RenderWindow& window,
    const std::vector<std::unique_ptr<figure>>& figures
);

void processEvents(
    sf::RenderWindow& window,
    Board* board,
    std::vector<std::unique_ptr<figure>>& figures,
    bool& isFigureSelected,
    figure*& selectedFigure,
    std::vector<std::pair<int, int>>& possibleMoves,
    sf::RectangleShape& lastMoveFrom,
    sf::RectangleShape& lastMoveTo,
    bool& hasMoved,
    float OFFSETX,
    float OFFSETY,
    float CELLSIZE
);

void drawMoveHighlights( // нарисовать все возможные ходы
    sf::RenderWindow& window,
    const std::vector<std::pair<int, int>>& possibleMoves,
    const Board& board,
    figure* selectedFigure,
    float OFFSETX,
    float OFFSETY,
    float CELLSIZE
);
// закрывает окно по крестику
void handleWindowClose(sf::RenderWindow& window, const sf::Event& event);

// пытается выбрать фигуру под курсором
void selectFigure(
    const sf::Vector2f& mousePos,
    Board* board,
    std::vector<std::unique_ptr<figure>>& figures,
    bool& isFigureSelected,
    figure*& selectedFigure,
    std::vector<std::pair<int, int>>& possibleMoves
);

// пытается ходить выбранной фигурой переставляет последний ход ( зеленые квадраты вот эти)
bool applyMoveIfValid(
    const sf::Vector2f& mousePos,
    Board* board,
    std::vector<std::unique_ptr<figure>>& figures,
    figure*& selectedFigure,
    const std::vector<std::pair<int, int>>& possibleMoves,
    sf::RectangleShape& lastMoveFrom,
    sf::RectangleShape& lastMoveTo,
    bool& hasMoved,
    float OFFSETX,
    float OFFSETY,
    float CELLSIZE
);

// если клик сначало по фигуре а потом не по клетке
void updateSelectionOnMissClick(
    const sf::Vector2f& mousePos,
    Board* board,
    std::vector<std::unique_ptr<figure>>& figures,
    figure*& selectedFigure,
    std::vector<std::pair<int, int>>& possibleMoves
);

