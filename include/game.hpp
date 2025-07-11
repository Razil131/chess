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

// загрузить текстуры
void loadTextures(std::map<std::string, sf::Texture>& textures);


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

void drawFigures(sf::RenderWindow& window, Board* board,  float CELLSIZE, float OFFSETX, float OFFSETY);  // нарисовать фигуры


void processEvents( // обработать все события на окне клик мыши закрытие окна
    sf::RenderWindow& window,
    Board* board,
    bool& endGameScreen,
    sf::RectangleShape newGameButtonRect,
    bool& isFigureSelected,
    figure*& selectedFigure,
    std::vector<std::pair<int, int>>& possibleMoves,
    sf::RectangleShape& lastMoveFrom,
    sf::RectangleShape& lastMoveTo,
    std::map<std::string, sf::Texture>& textures,
    std::vector<sf::Sprite>& to_choose,
    std::vector<sf::RectangleShape>& rectangles_to_choose,
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
    bool& isFigureSelected,
    figure*& selectedFigure,
    std::vector<std::pair<int, int>>& possibleMoves
);

// пытается ходить выбранной фигурой переставляет последний ход ( зеленые квадраты вот эти)
bool applyMoveIfValid(
    const sf::Vector2f& mousePos,
    Board* board,
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
    figure*& selectedFigure,
    std::vector<std::pair<int, int>>& possibleMoves
);

void drawCheck(sf::RenderWindow& window, // отрисовать красный квадрат на короле с шахом
    Board* board,
    figure::teams team,
    float OFFSETX,
    float OFFSETY,
    float CELLSIZE);

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

void drawEndGameScreen(sf::RenderWindow& window, //отрисовать меню завершения игры
    figure::teams winner,
    const sf::Font& font,
    sf::RectangleShape& btnRect);

void drawMainMenu(sf::RenderWindow& win, std::map<std::string, sf::RectangleShape>& btns, sf::Font& font); // отрисовать главное меню

void drawPuzzleMenu(sf::RenderWindow& win, // отрисовать меню с задачами
                    std::map<std::string, sf::RectangleShape>& numberBtns,
                    sf::RectangleShape& backBtn,
                    sf::RectangleShape& createBtn,
                    sf::Font& font,
                    int puzzleCount);

void drawGameTypeMenu(sf::RenderWindow& win, std::map<std::string, sf::RectangleShape>& btns, sf::RectangleShape& backBtn, sf::Font& font); // отрисовать выбор режима игры

void drawOpponentMenu(sf::RenderWindow& win, std::map<std::string, sf::RectangleShape>& btns, sf::RectangleShape& backBtn, sf::Font& font); // отрисовать меню выбора соперника ии или человек

void drawColorMenu(sf::RenderWindow& win, std::map<std::string, sf::RectangleShape>& btns, sf::RectangleShape& backBtn, sf::Font& font); // отрисовать выбор цвета команды