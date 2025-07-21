#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include "figure.hpp"
#include "modes.hpp"
#include "events.hpp"

sf::RectangleShape makeButton(float w, float h, sf::Color fill, sf::Color outline);

void drawLabel(sf::RenderWindow& win,const sf::Font& font,
               const sf::RectangleShape& btn, const std::string& s, unsigned size=24); // написать текст по центру кнопки

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

void createMainMenu(sf::RenderWindow& window, sf::Font& font); // создать меню для перезапуска после конца игры
