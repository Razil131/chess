#include "ui_board.hpp"

void initializeBoardRectangles(sf::RectangleShape boardRectangles[8][8], float CELLSIZE, float OFFSETX, float OFFSETY) {
    // создаем квадраты
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            boardRectangles[i][j] = sf::RectangleShape(sf::Vector2f(CELLSIZE, CELLSIZE)); // создание квадрата
            boardRectangles[i][j].setPosition(OFFSETX + i * CELLSIZE, OFFSETY + j * CELLSIZE); // на позицию его ставим так сказать
            if ((i + j) % 2 == 0) // так цвета заполняютя по четности
                boardRectangles[i][j].setFillColor(sf::Color(240,217,181)); // светлые
            else
                boardRectangles[i][j].setFillColor(sf::Color(181,136,99)); // темные
        }
    }
}

void createLetters(sf::Text letters[8], const sf::Font& font, float CELLSIZE, float OFFSETX, float OFFSETY) {
    // буквы под доской
    for (int i = 0; i < 8; i++) {
        letters[i].setFont(font); // ставим шрифт
        letters[i].setString(std::string(1, 'a' + i)); // ставим че писать
        letters[i].setCharacterSize(24); // размер шрифта
        letters[i].setFillColor(sf::Color::White); // цвет букв
        letters[i].setPosition(OFFSETX + i * CELLSIZE + CELLSIZE / 2 - 8, OFFSETY + 8 * CELLSIZE + 5); // расположение
    }
}

void createNumbers(sf::Text numbers[8], const sf::Font& font, float CELLSIZE, float OFFSETY) {
    // цифры слева
    for (int j = 0; j < 8; j++) {
        numbers[j].setFont(font); // ставим шрифт
        numbers[j].setString(std::to_string(8 - j)); // ставим че писать
        numbers[j].setCharacterSize(24); // размер шрифта
        numbers[j].setFillColor(sf::Color::White); // цвет букв
        numbers[j].setPosition(30, OFFSETY + j * CELLSIZE + CELLSIZE / 2 - 12); // расположение
    }
}

void drawBoardAndLabels(sf::RenderWindow& window, sf::RectangleShape boardRectangles[8][8], const sf::Text letters[8], const sf::Text numbers[8]) {
    // рисуем заново клетки
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            window.draw(boardRectangles[i][j]);
        }
    }

    // отрисовываем буквы и цифры
    for (int i = 0; i < 8; i++)
        window.draw(letters[i]);
    for (int j = 0; j < 8; j++)
        window.draw(numbers[j]);
}