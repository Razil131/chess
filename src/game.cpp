#include "game.hpp"

namespace { // это штука чтобы только из этого файла можно было функцию применить
    void addFigure( // добавить фигуру на gui
        std::unique_ptr<figure> fig, // указатель на фигуру
        std::vector<std::unique_ptr<figure>>& figures, // вектор всех фигур чтобы туда новую добавить
        float offsetX, // отступ по x для букв
        float offsetY, // отстпу по y для цифр
        float cellSize, // размер ячейки поля
        int cellX, // по x на какой ячейке должна быть фигура
        int cellY) // по y на какой ячейке должна быть фигура
    {
        fig->getSprite()->setScale(1.2f, 1.2f); // чуть увеличиваем размер иконки 
        fig->getSprite()->setPosition(
            offsetX + 10 + cellX * cellSize, // ставим на позицию +10 +15 это чтобы примерно по центру ячейки картинка была
            offsetY + 15 + cellY * cellSize
        ); 
        figures.push_back(std::move(fig)); // добавляем в вектор фигур
    }
}

void setupFigures(
    std::map<std::string, sf::Texture>& textures, // мапа название фигуры - ее текстура
    std::vector<std::unique_ptr<figure>>& figures, // вектор указателей на фигуры
    float offsetX, // отступ по x для букв
    float offsetY, // отстпу по y для цифр
    float cellSize) // размер ячейки поля
{
    const std::string types[] = { "p", "r", "n", "b", "q", "k" }; // это чтобы генерить название png в materials (36 строка генерит)
    const std::string colors[] = { "w", "b" };

    // загружаем текстуры
    for (const std::string& color : colors) {
        for (const std::string& type : types) {
            std::string key = type + color;
            std::string filename = "../materials/" + key + ".png";
            textures[key] = sf::Texture(); // заполняем мапу
            if (!textures[key].loadFromFile(filename)) {
                throw std::runtime_error("Failed to load " + filename); // если нет картинки ошибка
            }
        }
    }

    // белые пешки
    for (int i = 0; i < 8; i++) {
        auto pawn_w = std::make_unique<pawn>(
            figure::WHITE, std::make_pair(i,1), textures["pw"]);
        addFigure(std::move(pawn_w), figures, offsetX, offsetY, cellSize, i, 6);
    }

    // белые основные фигуры
    addFigure(std::make_unique<rook>(
                  figure::WHITE, std::make_pair(0,0), textures["rw"]),
              figures, offsetX, offsetY, cellSize, 0, 7);
    addFigure(std::make_unique<knight>(
                  figure::WHITE, std::make_pair(1,0), textures["nw"]),
              figures, offsetX, offsetY, cellSize, 1, 7);
    addFigure(std::make_unique<bishop>(
                  figure::WHITE, std::make_pair(2,0), textures["bw"]),
              figures, offsetX, offsetY, cellSize, 2, 7);
    addFigure(std::make_unique<queen>(
                  figure::WHITE, std::make_pair(3,0), textures["qw"]),
              figures, offsetX, offsetY, cellSize, 3, 7);
    addFigure(std::make_unique<king>(
                  figure::WHITE, std::make_pair(4,0), textures["kw"]),
              figures, offsetX, offsetY, cellSize, 4, 7);
    addFigure(std::make_unique<bishop>(
                  figure::WHITE, std::make_pair(5,0), textures["bw"]),
              figures, offsetX, offsetY, cellSize, 5, 7);
    addFigure(std::make_unique<knight>(
                  figure::WHITE, std::make_pair(6,0), textures["nw"]),
              figures, offsetX, offsetY, cellSize, 6, 7);
    addFigure(std::make_unique<rook>(
                  figure::WHITE, std::make_pair(7,0), textures["rw"]),
              figures, offsetX, offsetY, cellSize, 7, 7);

    // черные пешки
    for (int i = 0; i < 8; i++) {
        auto pawn_b = std::make_unique<pawn>(
            figure::BLACK, std::make_pair(i,6), textures["pb"]);
        addFigure(std::move(pawn_b), figures, offsetX, offsetY, cellSize, i, 1);
    }

    // черные основные фигуры
    addFigure(std::make_unique<rook>(
                  figure::BLACK, std::make_pair(0,7), textures["rb"]),
              figures, offsetX, offsetY, cellSize, 0, 0);
    addFigure(std::make_unique<knight>(
                  figure::BLACK, std::make_pair(1,7), textures["nb"]),
              figures, offsetX, offsetY, cellSize, 1, 0);
    addFigure(std::make_unique<bishop>(
                  figure::BLACK, std::make_pair(2,7), textures["bb"]),
              figures, offsetX, offsetY, cellSize, 2, 0);
    addFigure(std::make_unique<queen>(
                  figure::BLACK, std::make_pair(3,7), textures["qb"]),
              figures, offsetX, offsetY, cellSize, 3, 0);
    addFigure(std::make_unique<king>(
                  figure::BLACK, std::make_pair(4,7), textures["kb"]),
              figures, offsetX, offsetY, cellSize, 4, 0);
    addFigure(std::make_unique<bishop>(
                  figure::BLACK, std::make_pair(5,7), textures["bb"]),
              figures, offsetX, offsetY, cellSize, 5, 0);
    addFigure(std::make_unique<knight>(
                  figure::BLACK, std::make_pair(6,7), textures["nb"]),
              figures, offsetX, offsetY, cellSize, 6, 0);
    addFigure(std::make_unique<rook>(
                  figure::BLACK, std::make_pair(7,7), textures["rb"]),
              figures, offsetX, offsetY, cellSize, 7, 0);
}


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

bool loadFont(sf::Font& font) {
    // загружаем шрифт для букв и цифр
    if (!font.loadFromFile("../arial.ttf")) {
        return false; // если не загрузился то :(
    }
    return true;
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

void drawFigures(sf::RenderWindow& window, const std::vector<std::unique_ptr<figure>>& figures) {
    // рисуем фигуры
    for (auto& figura : figures) {
        window.draw(*(figura->getSprite()));
    }
}

void drawMoveHighlights( // рисуем все возможные ходы
    sf::RenderWindow& window,
    const std::vector<std::pair<int, int>>& possibleMoves,
    const Board& board,
    figure* selectedFigure,
    float OFFSETX,
    float OFFSETY,
    float CELLSIZE
) {
    for (const auto& move : possibleMoves) { // проходимся по всем возможным шагам
        int i = move.first;  // получаем x хз поч я их назвал i и j но мне оч лень их менят
        int j = move.second; // получаем y

        if (board.isOccupiedByEnemyTeam(i, j, selectedFigure->getTeam())) { // если на клетке враг отрисовываем 4 треугольника по углам клетки
            float TRIANGLESIZE = CELLSIZE / 5; // длина стороны треугольника
            sf::ConvexShape triangle; // это типо универсальная фигура можно поставить 5 точек можно 3 можно 100
            triangle.setPointCount(3); // ставим 3 точки
            triangle.setFillColor(sf::Color(0, 255, 0, 80)); // цвет треугольнк
            // верхний левый угол
            triangle.setPoint(0, sf::Vector2f(0, 0)); // ставим коорды трех точке относительно нулевой точки
            triangle.setPoint(1, sf::Vector2f(TRIANGLESIZE, 0));
            triangle.setPoint(2, sf::Vector2f(0, TRIANGLESIZE));
            triangle.setPosition(OFFSETX + i * CELLSIZE, OFFSETY + (7 - j) * CELLSIZE); // ставим треугольник
            window.draw(triangle); // рисуем ег
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
        } else {
            sf::CircleShape circle(CELLSIZE / 10); //  создаем круг с радиусом размер клетки/10
            circle.setFillColor(sf::Color(0, 255, 0, 80)); // цвет и прозрачность - 80/255
            circle.setPosition( // позицию ставим ему
                OFFSETX + i * CELLSIZE + (CELLSIZE / 2) - (CELLSIZE / 10),
                OFFSETY + (7 - j) * CELLSIZE + (CELLSIZE / 2) - (CELLSIZE / 10)
            );
            window.draw(circle); // и его отрисовываем
        }
    }
}




void processEvents(
    sf::RenderWindow& window,
    Board* board,
    std::vector<std::unique_ptr<figure>>& figures,
    bool& isFigureSelected,
    figure*& selectedFigure,
    std::vector<std::pair<int, int>>& possibleMoves,
    sf::RectangleShape& lastMoveFrom,
    sf::RectangleShape& lastMoveTo,
    bool& hasMoved,
    float OFFSETX,
    float OFFSETY,
    float CELLSIZE
) {
    sf::Event event;  // какое событие происходит сейчас клик мыши или закрытие окно
    while (window.pollEvent(event)) { // получаем постоянно событие какое то
        handleWindowClose(window, event); // чтобы закрывалось окно

        if (event.type == sf::Event::MouseButtonPressed && // нажатие левой кнопки мыши
            event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y); // получаем положение курсора

            if (!isFigureSelected) { // если фигура еще не выбрана
                selectFigure(mousePos, board, figures, // пытаемся выбрать фигуру по всем фигурам проходимся и смотрим (убрано в отдельную функцию)
                             isFigureSelected, selectedFigure,
                             possibleMoves);
            } else { // если уже была выбрана фигура ранее
                bool moved = applyMoveIfValid(  // двигаем фигуру на клетку на которую щелкнули передвигаем квадраты показывающие последний ход
                    mousePos, board, figures,
                    selectedFigure, possibleMoves,
                    lastMoveFrom, lastMoveTo,
                    hasMoved, OFFSETX,
                    OFFSETY, CELLSIZE
                );

                if (moved) { // если походили тоесть applyMoveifValid вернуло true
                    isFigureSelected = false; // фигура не выбрана
                    selectedFigure = nullptr; 
                    possibleMoves.clear(); // возможных ходов нет
                } else { // если движение не произошло тоесть клик был не по клетке а по фигуре или вообще вне поля
                    updateSelectionOnMissClick( // проверяем все фигуры вдруг по ним кликнули
                        mousePos, board, figures,
                        selectedFigure, possibleMoves
                    );
                }
            }
        }
    }
}

// закрывает окно по крестику
void handleWindowClose(sf::RenderWindow& window, const sf::Event& event) {
    if (event.type == sf::Event::Closed) {
        window.close();
    }
}

// пытается выбрать фигуру под курсором
void selectFigure(
    const sf::Vector2f& mousePos,
    Board* board,
    std::vector<std::unique_ptr<figure>>& figures,
    bool& isFigureSelected,
    figure*& selectedFigure,
    std::vector<std::pair<int, int>>& possibleMoves
) {
    for (auto& fig : figures) { // перебираем все фигуры
        if (fig->getSprite()->getGlobalBounds().contains(mousePos) && // если фигура под курсором и сейчас ход команды этой фигуры
            fig->getTeam() == board->getCurrentTeam()) {
            isFigureSelected = true; // фигура выбрана
            selectedFigure = fig.get(); 
            possibleMoves = selectedFigure->get_available_moves(*board);  // возможные ходы этой фигуры
            break;
        }
    }
}

// пытается ходить выбранной фигурой переставляет последний ход ( зеленые квадраты вот эти)
bool applyMoveIfValid(
    const sf::Vector2f& mousePos,
    Board* board,
    std::vector<std::unique_ptr<figure>>& figures,
    figure*& selectedFigure,
    const std::vector<std::pair<int, int>>& possibleMoves,
    sf::RectangleShape& lastMoveFrom,
    sf::RectangleShape& lastMoveTo,
    bool& hasMoved,
    float OFFSETX,
    float OFFSETY,
    float CELLSIZE
) {
    for (auto& move : possibleMoves) { // перебираем возможные ходы
        sf::FloatRect cellRect( // создаем квадратную область в каждом из возможных ходов чтобы потом проверять что мышка в этой области
            OFFSETX + move.first * CELLSIZE,
            OFFSETY + (7 - move.second) * CELLSIZE,
            CELLSIZE, CELLSIZE
        );
        if (!cellRect.contains(mousePos)) continue;  

        figure* target = board->getFigure(move.first, move.second); // получаем фигуру на месте куда надо сходить
        bool success = false; // выполнится ли хол
        if (target && target->getTeam() != selectedFigure->getTeam()) { //  если куда ходим стоит фигура противоположной команды
            if (board->makeMove(selectedFigure->getPos(), move)) { // ходим
                figures.erase( // убираем фигуру стоявшую на том месте куда сходили
                    std::remove_if( 
                        figures.begin(), figures.end(), // перебираем все фигуры в векторе figures
                        [&](const std::unique_ptr<figure>& f) { // анонимная функция возвращает true если позиция фигуры совпадает с позицией куда ходим
                            return f->getPos() == move; 
                        }
                    ),
                    figures.end() 
                );
                success = true; // ход успешно выполнен
            }
        } else {
            success = board->makeMove(selectedFigure->getPos(), move); // если куда ходим противника не было просто ходим
        }

        if (success) { // если ход выполнен перемещаем последний ход ( зеленые квадраты)
            lastMoveFrom.setPosition(
                OFFSETX + selectedFigure->getPos().first * CELLSIZE,
                OFFSETY + (7 - selectedFigure->getPos().second) * CELLSIZE
            );
            lastMoveTo.setPosition(
                OFFSETX + move.first * CELLSIZE,
                OFFSETY + (7 - move.second) * CELLSIZE
            );
            selectedFigure->setPos(move);
            selectedFigure->getSprite()->setPosition(
                OFFSETX + 10 + move.first * CELLSIZE,
                OFFSETY + 15 + (7 - move.second) * CELLSIZE
            );

            hasMoved = true;
            return true; // сходили
        }
    }
    return false; // не сходили :(
}

// если клик сначало по фигуре а потом не по клетке
void updateSelectionOnMissClick(
    const sf::Vector2f& mousePos,
    Board* board,
    std::vector<std::unique_ptr<figure>>& figures,
    figure*& selectedFigure,
    std::vector<std::pair<int, int>>& possibleMoves
) {
    for (auto& fig : figures) { // проверяем вдруг клик по другой фигуре
        if (fig->getSprite()->getGlobalBounds().contains(mousePos) && // если мышка на фигуре и эта фигура цвета который щас ходит
            fig->getTeam() == board->getCurrentTeam()) {
            selectedFigure = fig.get(); // получаем эту фигуру
            possibleMoves = selectedFigure->get_available_moves(*board);
            break;
        }
    }
}
