#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <vector>
#include "board.hpp"
#include "figure.hpp"
#include "menu.hpp"
#include "save.hpp"
#include "choice_menu.hpp"

void processEvents( // обработать все события на окне клик мыши закрытие окна
    sf::RenderWindow& window,
    sf::Font& font,
    Board* board,
    int mode,
    int players,
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
    float CELLSIZE,
    sf::RectangleShape* rightCastle = nullptr, // только для fisher
    sf::RectangleShape* leftCastle = nullptr // только для fisher
);

// закрывает окно по крестику
void handleWindowClose(sf::RenderWindow& window, const sf::Event& event, Board* board = nullptr, int players = 1, int mode = 1);

// обработать колесо мыши и нажатие по сохранению в savemenu
std::string handleSaveMenuEvents(sf::RenderWindow& win,
    sf::Event& event,
    std::map<std::string, sf::RectangleShape>& btns,
    float& scrollOffset,
    size_t savesCount,
    float buttonHeight = 40.f,
    float gap = 10.f,
    float scrollSpeed = 20.f
);

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

// обработать ивенты в меню создания задач
int handleCreatePuzzleEvents(const sf::Vector2f& mousePos,
    const sf::RectangleShape& container,
    int cellSize);