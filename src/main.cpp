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

    std::map<std::string, sf::Texture> textures; // мапа текстур
    loadTextures(textures);
    Board* board = new Board();  // создание твоей доски (❁´◡`❁)
    board->initialize(textures); // расставление фигур на твоей доске (╯°□°）╯︵ ┻━┻

    bool isFigureSelected = false; // фигура сейчас выбрана для хода
    figure* selectedFigure = nullptr; 
    std::vector<std::pair<int, int>> possibleMoves; // возможные ходы для выбранной фигуры

    std::vector<sf::Sprite> to_choose; // спрайты фигур в меню выбора
    std::vector<sf::RectangleShape> rectangles_to_choose; // прямоугольники на заднем плане в меню выбора

    sf::RectangleShape lastMoveFrom(sf::Vector2f(CELLSIZE, CELLSIZE)); // квадратик откуда последний ход
    sf::RectangleShape lastMoveTo(sf::Vector2f(CELLSIZE, CELLSIZE)); // квадратик куда последний ход
    lastMoveFrom.setFillColor(sf::Color(0, 255, 0, 80)); // цвет
    lastMoveTo.setFillColor(sf::Color(0, 255, 0, 80)); // цвет
    bool hasMoved = false; // флаг был ли уже ход а то при запуске когда хода не было сделано эти квадраты просто на угол уезжали и закрывали часть окна

    while (window.isOpen()) { // основной цикл постоянно повторяется пока окно открыто
        processEvents(window, board, isFigureSelected, selectedFigure, possibleMoves, lastMoveFrom, lastMoveTo, textures, to_choose, rectangles_to_choose, hasMoved, OFFSETX, OFFSETY, CELLSIZE); // обрабатываем все возможные события клик мыши и тд

        window.clear(sf::Color(128,128,128)); // отчищаем окно чтобы оно обновлялось цвет в скобках это цвет фона (серый)

        drawBoardAndLabels(window, boardRectangles, letters, numbers); // рисуем доску и цифры буквы
        if (hasMoved) { // если ход был рисуем зеленые квадраты на последнем ходу
            window.draw(lastMoveFrom);
            window.draw(lastMoveTo);
        }
        drawFigures(window, board, CELLSIZE, OFFSETX, OFFSETY); // рисуем фигуры

        if (isFigureSelected) { // если фигура выбрана
            drawMoveHighlights(window, possibleMoves, *board, selectedFigure, OFFSETX, OFFSETY, CELLSIZE); // рисуем возможные ходы
        }

        if (board->convertFlag){ // если пешка на клетке для превращения
            createChoiceMenu(board, to_choose, rectangles_to_choose, textures, OFFSETX, OFFSETY, CELLSIZE); // создаем и отрисовываем меню выбора
            drawChoiceMenu(window,to_choose, rectangles_to_choose);
        }

        window.display(); // показывалось окно чтобы
    }

    delete board; // отчищаем память от твоей доски 😥😥😣😣😥
    return 0;
}