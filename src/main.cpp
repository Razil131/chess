#include <SFML/Graphics.hpp>
#include "game.hpp"
#include "board.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(900, 900), "Chess"); // создаем окошечно так сказать 900 на 900 пикселей с название Chess

    const float CELLSIZE = 100.f; // размер клетки
    const float OFFSETX = 50.f; // отстпуп для букв слева
    const float OFFSETY = 50.f; // отступ для цифр снизу

    // создаем доску
    sf::RectangleShape boardRectangles[8][8];
    initializeBoardRectangles(boardRectangles, CELLSIZE, OFFSETX, OFFSETY);

    sf::Font font; // шрифт
    if (!loadFont(font)) {
        return -1; // если не загрузился то :(
    }

    //буквы
    sf::Text letters[8];
    createLetters(letters, font, CELLSIZE, OFFSETX, OFFSETY);

    //цифоы
    sf::Text numbers[8];
    createNumbers(numbers, font, CELLSIZE, OFFSETY);

    // мапа текстур и вектор фигур
    std::map<std::string, sf::Texture> textures;
    std::vector<std::unique_ptr<figure>> figures;
    setupFigures(textures, figures, OFFSETX, OFFSETY, CELLSIZE); // расставляет фигуры в gui

    bool isFigureSelected = false; // фигура сейчас выбрана для хода
    figure* selectedFigure = nullptr; 
    std::vector<std::pair<int, int>> possibleMoves; // возможные ходы для выбранной фигуры

    sf::RectangleShape lastMoveFrom(sf::Vector2f(CELLSIZE, CELLSIZE)); // квадратик откуда последний ход
    sf::RectangleShape lastMoveTo(sf::Vector2f(CELLSIZE, CELLSIZE)); // квадратик куда последний ход
    lastMoveFrom.setFillColor(sf::Color(0, 255, 0, 80)); // цвет
    lastMoveTo.setFillColor(sf::Color(0, 255, 0, 80)); // цвет
    bool hasMoved = false; // флаг был ли уже ход а то при запуске когда хода не было сделано эти квадраты просто на угол уезжали и закрывали часть окна

    Board* board = new Board();  // создание твоей доски (❁´◡`❁)
    board->initialize(); // расставление фигур на твоей доске (╯°□°）╯︵ ┻━┻

    while (window.isOpen()) { // основной цикл постоянно повторяется пока окно открыто
        processEvents(window, board, figures, isFigureSelected, selectedFigure, possibleMoves, lastMoveFrom, lastMoveTo, hasMoved, OFFSETX, OFFSETY, CELLSIZE); // обрабатываем все возможные события клик мыши и тд

        window.clear(sf::Color(128,128,128)); // отчищаем окно чтобы оно обновлялось цвет в скобках это цвет фона (серый)

        drawBoardAndLabels(window, boardRectangles, letters, numbers); // рисуем доску и цифры буквы
        if (hasMoved) { // если ход был рисуем зеленые квадраты на последнем ходу
            window.draw(lastMoveFrom);
            window.draw(lastMoveTo);
        }
        drawFigures(window, figures); // рисуем фигуры

        if (isFigureSelected) { // если фигура выбрана
            drawMoveHighlights(window, possibleMoves, *board, selectedFigure, OFFSETX, OFFSETY, CELLSIZE); // рисуем возможные ходы
        }


        window.display(); // показывалось окно чтобы
    }

    delete board; // отчищаем память от твоей доски 😥😥😣😣😥
    return 0;
}