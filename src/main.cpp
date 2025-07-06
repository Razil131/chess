#include <SFML/Graphics.hpp>
#include <vector>
#include "game.hpp"
#include "board.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(900, 900), "Chess"); // создаем окошечно так сказать 900 на 900 пикселей с название Chess

    const float CELLSIZE = 100.f; // размер клетки
    const float OFFSETX = 50.f; // отстпуп для букв слева
    const float OFFSETY = 50.f; // отступ для цифр снизу

    // создаем квадраты
    sf::RectangleShape boardRectangles[8][8];
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

    // загружаем шрифт для букв и цифр
    sf::Font font;
    if (!font.loadFromFile("../arial.ttf")) {
        return -1; // если не загрузился то :(
    }

    // буквы под доской
    sf::Text letters[8]; // массив с объектами текста
    for (int i = 0; i < 8; i++) {
        letters[i].setFont(font); // ставим шрифт
        letters[i].setString(std::string(1, 'a' + i)); // ставим че писать
        letters[i].setCharacterSize(24); // размер шрифта
        letters[i].setFillColor(sf::Color::White); // цвет букв
        letters[i].setPosition(OFFSETX + i * CELLSIZE + CELLSIZE / 2 - 8, OFFSETY + 8 * CELLSIZE + 5); // расположение
    }

    // цифры слева
    sf::Text numbers[8]; // массив с объектами текста
    for (int j = 0; j < 8; j++) { 
        numbers[j].setFont(font); // ставим шрифт
        numbers[j].setString(std::to_string(8 - j)); // ставим че писать
        numbers[j].setCharacterSize(24); // размер шрифта
        numbers[j].setFillColor(sf::Color::White); // цвет букв
        numbers[j].setPosition(30, OFFSETY + j * CELLSIZE + CELLSIZE / 2 - 12); // расположение
    }

    // загружаем фигуры
    std::map<std::string, sf::Texture> textures; // мапа которую передадим в функцию
    std::vector<std::unique_ptr<figure>> figures; // и вектор фигур который тоже передадим
    setupFigures(textures, figures, OFFSETX, OFFSETY, CELLSIZE); // расставляет фигуры в gui

    // переменные чтобы когда на фигуры тыкаешь выбор хода появлялся
    bool isFigureSelected = false; // флаг фигура выбрана
    figure* selectedFigure = nullptr; // и указатель на выбранную фигуру
    std::vector<std::pair<int, int>> possibleMoves; // ходы возможные для выбранной фигуры

    // подсветка последних ходов
    sf::RectangleShape lastMoveFrom(sf::Vector2f(CELLSIZE, CELLSIZE)); // откуда последний ход это если что квадраты зеленые которые перемещаются при ходе
    sf::RectangleShape lastMoveTo(sf::Vector2f(CELLSIZE, CELLSIZE)); // куда последний ход
    lastMoveFrom.setFillColor(sf::Color(0, 255, 0, 80)); // цвет 80 - прозрачность
    lastMoveTo.setFillColor(sf::Color(0, 255, 0, 80));   // цвет 80 - прозрачность
    bool hasMoved = false; // флаг был ли уже ход а то при запуске когда хода не было сделано эти квадраты просто на угол уезжали и закрывали часть окна

    Board* board = new Board(); // создание твоей доски (❁´◡`❁)
    board->initialize(); // расставление фигур на твоей доске (╯°□°）╯︵ ┻━┻

    while (window.isOpen()) { // основной цикл постоянно повторяется пока окно открыто
        sf::Event event; // какое событие происходит сейчас клик мыши или закрытие окно
        while (window.pollEvent(event)) {   // получаем постоянно событие какое то
            if (event.type == sf::Event::Closed) { // чтобы закрывалось окно
                window.close(); 
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) { // нажатие левой кнопки мыши
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y); // получаем положение курсора

                if (!isFigureSelected) { // если фигура еще не выбрана
                    for (auto& figura : figures) { // пытаемся выбрать фигуру по всем фигурам проходимся и смотрим 
                        if (figura->getSprite()->getGlobalBounds().contains(mousePos)) { // если мышка на фигуре какой то
                            isFigureSelected = true; // то выбрана фигура
                            selectedFigure = figura.get(); // получаем фигуру
                            possibleMoves = selectedFigure->get_available_moves(*board); // прощитываем для нее возможные ходы
                            break;
                        }
                    }
                } else { // если уже была выбрана фигура ранее
                    bool moved = false; // еще не двигались
                    for (std::pair<int, int>& move : possibleMoves) { // перебираем все возможные ходы
                        sf::FloatRect cellRect( // создаем квадратную область в каждом из возможных ходов чтобы потом проверять что мышка в этой области
                            OFFSETX + move.first * CELLSIZE,
                            OFFSETY + (7 - move.second) * CELLSIZE,
                            CELLSIZE, CELLSIZE
                        );
                        if (cellRect.contains(mousePos)) { // если мышка в области этого квадрата который расположен на каком то возможном ходу
                            if (board->makeMove(selectedFigure->getPos(), move)) { // делаем ход с помощью твоей функции ╰(*°▽°*)╯ которая возвращает true если ход сделан
                                // обновляем позицию подсветки последних ходов
                                lastMoveFrom.setPosition(OFFSETX + selectedFigure->getPos().first * CELLSIZE, OFFSETY + (7 - selectedFigure->getPos().second) * CELLSIZE);
                                lastMoveTo.setPosition(OFFSETX + move.first * CELLSIZE, OFFSETY + (7 - move.second) * CELLSIZE);
                                selectedFigure->setPos(move); // ставим фигуру на новое место на backend (T_T)
                                selectedFigure->getSprite()->setPosition( // и на gui (⊙_⊙;)
                                    OFFSETX + 10 + move.first * CELLSIZE,
                                    OFFSETY + 15 + (7 - move.second) * CELLSIZE
                                );
                                moved = true; // движение произошло
                                hasMoved = true; // квадраты по последнему ходу появились чтобы
                            }
                            break;
                        }
                    }

                    if (moved) { // если движение произошло
                        isFigureSelected = false; // отчищаем все показатели так сказать
                        selectedFigure = nullptr;
                        possibleMoves.clear();
                    } else {
                        // если движение не произошло тоесть клик был не по клетке а по фигуре или вообще вне поля
                        for (auto& figura : figures) { // проверяем все фигуры вдруг по ним кликнули
                            if (figura->getSprite()->getGlobalBounds().contains(mousePos)) { // если по фигуре кликнули 
                                selectedFigure = figura.get(); // получаем фигуру
                                possibleMoves = selectedFigure->get_available_moves(*board); // получаем ее возможные ходы
                                break;
                            }
                        }
                    }
                }
            }
        }

        window.clear(sf::Color(128,128,128)); // отчищаем окно чтобы оно обновлялось цвет в скобках это цвет фона (серый)

        // рисуем заного клетки уже созданные
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                window.draw(boardRectangles[i][j]);
            }
        }

        // отрисовываем подсветку последнего хода
        if (hasMoved) {
            window.draw(lastMoveFrom);
            window.draw(lastMoveTo);
        }

        // рисуем фигуры
        for (auto& figure : figures) {
            window.draw(*(figure->getSprite()));
        }

        // рисуем круги куда можно ходить и вот эти треугольники когда на противника ходишь
        if (isFigureSelected) { // если фигура выбрана
            for (const auto& move : possibleMoves) { // проходимся по всем возможным шагам
                int i = move.first; // получаем x хз поч я их назвал i и j но мне оч лень их менять
                int j = move.second; // получаем y
                if (board->isOccupiedByEnemyTeam(i, j, selectedFigure->getTeam())) { // если на клетке враг отрисовываем 4 треугольника по углам клетки
                    float TRIANGLESIZE = CELLSIZE / 5; // длина стороны треугольника
                    sf::ConvexShape triangle; // это типо универсальная фигура можно поставить 5 точек можно 3 можно 100
                    triangle.setPointCount(3); // ставим 3 точки
                    triangle.setFillColor(sf::Color(0, 255, 0, 80)); // цвет треугольнка

                    // верхний левый угол
                    triangle.setPoint(0, sf::Vector2f(0, 0)); // ставим коорды трех точке относительно нулевой точки
                    triangle.setPoint(1, sf::Vector2f(TRIANGLESIZE, 0));
                    triangle.setPoint(2, sf::Vector2f(0, TRIANGLESIZE));
                    triangle.setPosition(OFFSETX + i * CELLSIZE, OFFSETY + (7 - j) * CELLSIZE); // ставим треугольник
                    window.draw(triangle); // рисуем его

                    // верхний правый угол
                    triangle.setPoint(0, sf::Vector2f(CELLSIZE, 0)); // ставим коорды трех точке относительно нулевой точки
                    triangle.setPoint(1, sf::Vector2f(CELLSIZE - TRIANGLESIZE, 0));
                    triangle.setPoint(2, sf::Vector2f(CELLSIZE, TRIANGLESIZE));
                    triangle.setPosition(OFFSETX + i * CELLSIZE, OFFSETY + (7 - j) * CELLSIZE); // ставим треугольник
                    window.draw(triangle);

                    // нижний левый угол
                    triangle.setPoint(0, sf::Vector2f(0, CELLSIZE)); // ставим коорды трех точке относительно нулевой точки
                    triangle.setPoint(1, sf::Vector2f(TRIANGLESIZE, CELLSIZE));
                    triangle.setPoint(2, sf::Vector2f(0, CELLSIZE - TRIANGLESIZE));
                    triangle.setPosition(OFFSETX + i * CELLSIZE, OFFSETY + (7 - j) * CELLSIZE); // ставим треугольник
                    window.draw(triangle);

                    // нижний правый угол
                    triangle.setPoint(0, sf::Vector2f(CELLSIZE, CELLSIZE)); // ставим коорды трех точке относительно нулевой точки
                    triangle.setPoint(1, sf::Vector2f(CELLSIZE - TRIANGLESIZE, CELLSIZE));
                    triangle.setPoint(2, sf::Vector2f(CELLSIZE, CELLSIZE - TRIANGLESIZE)); 
                    triangle.setPosition(OFFSETX + i * CELLSIZE, OFFSETY + (7 - j) * CELLSIZE); // ставим треугольник
                    window.draw(triangle);

                    continue; // пропускаем отрисовку зеленого круга
                }

                sf::CircleShape circle(CELLSIZE / 10); //  создаем круг с радиусом размер клетки/10
                circle.setFillColor(sf::Color(0, 255, 0, 80)); // цвет и прозрачность - 80/255
                circle.setPosition( // позицию ставим ему
                    OFFSETX + i * CELLSIZE + (CELLSIZE / 2) - (CELLSIZE / 10),
                    OFFSETY + (7 - j) * CELLSIZE + (CELLSIZE / 2) - (CELLSIZE / 10)
                );
                window.draw(circle); // и его отрисовываем
            }
        }

        // отрисосываем буквы и цифры
        for (int i = 0; i < 8; i++)
            window.draw(letters[i]);
        for (int j = 0; j < 8; j++)
            window.draw(numbers[j]);

        window.display(); // показывалось окно чтобы
    }

    delete board; // отчищаем память от твоей доски 😥😥😣😣😥
    return 0;
}