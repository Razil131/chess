#include <SFML/Graphics.hpp>
#include "../include/game.hpp"



int main() {
    sf::RenderWindow window(sf::VideoMode(900, 900), "chess"); // окно 900 на 900
   


    const float CELLSIZE = 100.f; // размер ячейки
    const float OFFSETX = 50.f; // смещаем доску вправо чтобы буквы и цифры поместились
    const float OFFSETY = 50.f; // смещаем доску вниз

    sf::RectangleShape boardRectangles[8][8]; // массив для хранения квадратов из которых состоит поле

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            boardRectangles[i][j] = sf::RectangleShape(sf::Vector2f(CELLSIZE, CELLSIZE));// создаем квадрат
            boardRectangles[i][j].setPosition(OFFSETX + i * CELLSIZE, OFFSETY + j * CELLSIZE); // ставим квадрат на правильную позицию

            // цвет шахматной клетки
            if ((i + j) % 2 == 0)
                boardRectangles[i][j].setFillColor(sf::Color::White);
            else
                boardRectangles[i][j].setFillColor(sf::Color(25,25,25)); // не черный чтобы видно было черные фигуры
        }
    }

    // загружаем шрифт
    sf::Font font;
    if (!font.loadFromFile("../arial.ttf")) {
        return -1; // если шрифт не загрузился
    }

    // буквы под доской
    sf::Text letters[8];
    for (int i = 0; i < 8; i++) {
        letters[i].setFont(font); // шрифт
        letters[i].setString(std::string(1, 'a' + i)); // текст: буква
        letters[i].setCharacterSize(24); // размер
        letters[i].setFillColor(sf::Color::White); // цвет
        letters[i].setPosition(OFFSETX + i * CELLSIZE + CELLSIZE / 2 - 8, OFFSETY + 8 * CELLSIZE + 5); // позиция
    }

    // цифры слева от доски
    sf::Text numbers[8];
    for (int j = 0; j < 8; j++) {
        numbers[j].setFont(font); // шрифт
        numbers[j].setString(std::to_string(8 - j)); // текст: цифра
        numbers[j].setCharacterSize(24); // размер
        numbers[j].setFillColor(sf::Color::White); // цвет
        numbers[j].setPosition(30, OFFSETY + j * CELLSIZE + CELLSIZE / 2 - 12); // позиция
    }
    
    // Загружаем фигуры
    std::map<std::string, sf::Texture> textures;
    std::vector<ChessFigure> figures;
    setupFigures(textures, figures, OFFSETX, OFFSETY, CELLSIZE);

    while (window.isOpen()) { // основной цикл
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) // закрытие окна
                window.close();
        }

        window.clear(sf::Color(128,128,128));  // обновление окна и ставится синий фон
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                window.draw(boardRectangles[i][j]); // отрисовываем снова все квадраты
            }
        }

        // отрисовываем буквы
        for (int i = 0; i < 8; i++) {
            window.draw(letters[i]);
        }

        // отрисовываем цифры
        for (int j = 0; j < 8; j++) {
            window.draw(numbers[j]);
        }
        // отрисовывает фигуры
        for (auto& figure : figures) {
            window.draw(figure.sprite);
        }

        window.display();
    }

    return 0;
}
