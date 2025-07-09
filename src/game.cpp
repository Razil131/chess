#include "game.hpp"


void loadTextures(std::map<std::string, sf::Texture>& textures){
    const std::string types[] = {"b", "p", "r", "n", "q", "k" }; // это чтобы генерить название png в materials
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

void drawFigures(sf::RenderWindow& window, Board* board,  float CELLSIZE, float OFFSETX, float OFFSETY) {
    // рисуем фигуры
    for (const auto& row : *(board->getBoard())) {
        for (const auto& fig : row) {
            if (fig) {
                fig->getSprite()->setPosition(
                    OFFSETX + 10 + (fig->getPos().first) * CELLSIZE, // ставим на позицию +10 +15 это чтобы примерно по центру ячейки картинка была
                    OFFSETY + 15 + (7-fig->getPos().second) * CELLSIZE
                ); 
                window.draw(*(fig->getSprite()));
            }
        }
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
    bool& isFigureSelected,
    figure*& selectedFigure,
    std::vector<std::pair<int, int>>& possibleMoves,
    sf::RectangleShape& lastMoveFrom,
    sf::RectangleShape& lastMoveTo,
    std::map<std::string, sf::Texture>& textures,
    std::vector<sf::Sprite>& to_choose,
    std::vector<sf::RectangleShape>& rectangles_to_choose,
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
            // если пешка на клетке для превращения
            if (board->convertFlag){
                selectFigureToConvert(board, rectangles_to_choose, mousePos, textures, OFFSETX, CELLSIZE); // выбираем и превращаем
                return;
            }
            if (!isFigureSelected) { // если фигура еще не выбрана
                selectFigure(mousePos, board, // пытаемся выбрать фигуру по всем фигурам проходимся и смотрим (убрано в отдельную функцию)
                             isFigureSelected, selectedFigure,
                             possibleMoves);
            } else { // если уже была выбрана фигура ранее
                bool moved = applyMoveIfValid(  // двигаем фигуру на клетку на которую щелкнули передвигаем квадраты показывающие последний ход
                    mousePos, board,
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
                        mousePos, board,
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
    bool& isFigureSelected,
    figure*& selectedFigure,
    std::vector<std::pair<int, int>>& possibleMoves
) {

    for (const auto& row : *(board->getBoard())) {// перебираем все фигуры
        for (const auto& fig : row) {
            if (fig) {
                if (fig->getSprite()->getGlobalBounds().contains(mousePos) && // если фигура под курсором и сейчас ход команды этой фигуры
                    fig->getTeam() == board->getCurrentTeam()) {
                    isFigureSelected = true; // фигура выбрана
                    selectedFigure = fig.get();
                    std::pair<int, int> boardPos = selectedFigure->getPos();
                    possibleMoves = board->getValidMoves(boardPos.first, boardPos.second);;  // возможные ходы этой фигуры
                    break;
                }
            }
        }
        if (isFigureSelected) break;  // выход из внешнего цикла, если фигура найдена
    }
}

// пытается ходить выбранной фигурой переставляет последний ход ( зеленые квадраты вот эти)
bool applyMoveIfValid(
    const sf::Vector2f& mousePos,
    Board* board,
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

        std::pair<int,int> prev_pos = selectedFigure->getPos();
        if (board->makeMove(selectedFigure->getPos(), move)) { // ходим
            lastMoveFrom.setPosition(
            OFFSETX + prev_pos.first * CELLSIZE,
            OFFSETY + (7 - prev_pos.second) * CELLSIZE
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
    figure*& selectedFigure,
    std::vector<std::pair<int, int>>& possibleMoves
) {

    bool found = false;
    for (const auto& row : *(board->getBoard())) {// перебираем все фигуры
        for (const auto& fig : row) {
            if (fig) {
                if (fig->getSprite()->getGlobalBounds().contains(mousePos) && // если фигура под курсором и сейчас ход команды этой фигуры
                    fig->getTeam() == board->getCurrentTeam()) {
                    selectedFigure = fig.get();
                    found = true; 
                    std::pair<int, int> boardPos = selectedFigure->getPos();
                    possibleMoves = board->getValidMoves(boardPos.first, boardPos.second);;  // возможные ходы этой фигуры
                    break;
                }
            }

        }
        if (found) break;  // выход из внешнего цикла
    }
}

// создать меню выбора
void createChoiceMenu(
    const Board* board,
    std::vector<sf::Sprite>& to_choose,
    std::vector<sf::RectangleShape>& rectangles_to_choose,
    std::map<std::string, sf::Texture>& textures,
    float OFFSETX,
    float OFFSETY,
    float CELLSIZE
) {
    to_choose.clear(); // отчищаем прошлое меню
    rectangles_to_choose.clear();

    // получаем позицию для превращения
    std::pair<int, int> pos = board->convertPosition;
    figure* pawn = board->getFigure(pos);

    // определяем цвет
    std::string colorPrefix = (pawn->getTeam() == figure::WHITE) ? "w" : "b";

    // названия фигур для превращения
    std::vector<std::string> types = { "q", "b", "n", "r" };

    for (int i = 0; i < types.size(); ++i) {
        sf::Sprite sprite; // создаем спрайт фигуры
        sprite.setTexture(textures[types[i] + colorPrefix]);
        sprite.setScale(1.2f, 1.2f);

        float x = OFFSETX + (2 + i) * CELLSIZE + 10;
        float y = OFFSETY + 3.5f * CELLSIZE;

        sprite.setPosition(x, y);
        to_choose.push_back(sprite);

        // создаём фон за фигурой
        sf::RectangleShape rect(sf::Vector2f(CELLSIZE, CELLSIZE));
        rect.setPosition(x - 10, y - 15); // чуть левее и выше чтобы обрамить спрайт
        rect.setFillColor(sf::Color(200, 200, 200, 180)); // светло-серый с прозрачностью
        rect.setOutlineColor(sf::Color::Black);
        rect.setOutlineThickness(2.f);
        rectangles_to_choose.push_back(rect);
    }
}

// отрисовать меню выбора
void drawChoiceMenu(sf::RenderWindow& window,std::vector<sf::Sprite>& to_choose, std::vector<sf::RectangleShape> rectanges_to_choose){
    for (sf::RectangleShape rect: rectanges_to_choose){ // рисуем задний фон
        window.draw(rect);
    }
    for (sf::Sprite sprite:to_choose){ // рисуем спрайты фигур
        window.draw(sprite);
    }
}

// выбрать и предвратить фигуру
void selectFigureToConvert(Board* board,
    const std::vector<sf::RectangleShape> rectangles_to_choose,
    const sf::Vector2f& mousePos,
    std::map<std::string, sf::Texture>& textures,
    float OFFSETX,
    float CELLSIZE){
    figure::teams col = board->getFigure(board->convertPosition)->getTeam(); // цвет фигуры
    for (sf::RectangleShape rect: rectangles_to_choose){ // перебираем квадраты
        if (rect.getGlobalBounds().contains(mousePos)){ // если мышка в этом квадрате
            int rect_col = ((rect.getPosition().x-OFFSETX-10)/CELLSIZE)-1;
            switch (rect_col){
                case (0): // первый квадрат то делаем королевой так как ее спрайт в первом квадрате
                    board->convertPawn(board->convertPosition.first, board->convertPosition.second, figure::QUEEN, col==figure::WHITE ? textures["qw"] : textures["qb"]);
                    break;
                case(1): // второй квадрат и так далее
                    board->convertPawn(board->convertPosition.first, board->convertPosition.second, figure::BISHOP, col==figure::WHITE ? textures["bw"] : textures["bb"]);
                    break;
                case(2):
                    board->convertPawn(board->convertPosition.first, board->convertPosition.second, figure::KNIGHT, col==figure::WHITE ? textures["nw"] : textures["nb"]);
                    break;
                case(3):
                    board->convertPawn(board->convertPosition.first, board->convertPosition.second, figure::ROOK, col==figure::WHITE ? textures["rw"] : textures["rb"]);
                    break;
            }
        }
    }
}