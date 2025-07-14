#include <SFML/Graphics.hpp>
#include <iostream>
#include "menu.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(900, 900), "Chess", sf::Style::Titlebar | sf::Style::Close); // создаем окошечно так сказать 900 на 900 пикселей с название Chess

   sf::Font font; // шрифт
   if (!loadFont(font)) {
       return -1; // если не загрузился то :(
   }

   createMainMenu(window,font); // создаем меню
    return 0;
}