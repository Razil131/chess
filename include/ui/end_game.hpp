#pragma once

#include <SFML/Graphics.hpp>
#include "figure.hpp"
#include "menu.hpp"

void drawEndGameScreen(sf::RenderWindow& window, //отрисовать меню завершения игры
    figure::teams winner,
    const sf::Font& font,
    sf::RectangleShape& btnRect);

void drawEndGameScreenPuzzle(sf::RenderWindow& window, //отрисовать меню завершения игры в пазлах
    bool win,
    const sf::Font& font,
    sf::RectangleShape& btnRect);