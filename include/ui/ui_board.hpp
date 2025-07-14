#pragma once

#include <SFML/Graphics.hpp>

void initializeBoardRectangles( // расставить все клетки доски
    sf::RectangleShape boardRectangles[8][8],
    float CELLSIZE,
    float OFFSETX,
    float OFFSETY
);

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