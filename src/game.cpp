#include "game.hpp"
#include <functional>


sf::RectangleShape makeButton(float w, float h, sf::Color fill, sf::Color outline) { // функция создания кнопки
    sf::RectangleShape btn({w, h}); // создаем кнопки с заданой шириной и высотой
    btn.setFillColor(fill); // цвет
    btn.setOutlineColor(outline); // цвет рамки
    btn.setOutlineThickness(2.f); // ширина рамки
    btn.setOrigin(w/2, h/2); // ставим начало коорд у кнопки в ее середину чтобы удобно поставить ее в центр экрана например
    return btn;
}


void drawLabel(sf::RenderWindow& win,const sf::Font& font,
               const sf::RectangleShape& btn, const std::string& s, unsigned size=24) // написать текст по центру кнопки
{
    sf::Text t(s, font, size); // текст
    t.setFillColor(sf::Color::Black); // цвет текста
    auto bb = t.getLocalBounds(); // квадрат в котором текст и ставим начальные коорды кнопки в его центр но чуть чуть выше
    t.setOrigin(bb.width/2, bb.height/2 + 5); // +5 потому что текст кажется не по центру а ниже
    t.setPosition(btn.getPosition()); // ставим коорды текста в кнопку
    win.draw(t); // рисуем текст
}

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
    bool& endGameScreen,
    sf::RectangleShape newGameButtonRect,
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
    float CELLSIZE,
    sf::RectangleShape* rightCastle,
    sf::RectangleShape* leftCastle 
) {
    sf::Event event;  // какое событие происходит сейчас клик мыши или закрытие окно
    while (window.pollEvent(event)) { // получаем постоянно событие какое то
        handleWindowClose(window, event); // чтобы закрывалось окно

        if (event.type == sf::Event::MouseButtonPressed && // нажатие левой кнопки мыши
            event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y); // получаем положение курсора
            if (endGameScreen){ // если игра закончилась проверяем нажата ли кнопка начать новую игру и начинаем ее
                if (newGameButtonRect.getGlobalBounds().contains(mousePos)){
                    board->initialize(textures);
                    endGameScreen = false;
                    hasMoved = false;
                }
                return;
            }
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
            if (leftCastle and rightCastle){ // если существуют кнопки рокировки -> режим фишера
                if ((*leftCastle).getGlobalBounds().contains(mousePos)) // если нажали на кнопки рокировки в фишере
                    board->fisherCastle(false);
                else if ((*rightCastle).getGlobalBounds().contains(mousePos))
                    board->fisherCastle(true);
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

void drawCheck(sf::RenderWindow& window, // нарисовать красный квадрат на короле с шахом
    Board* board,
    figure::teams team,
    float OFFSETX,
    float OFFSETY,
    float CELLSIZE){
    sf::RectangleShape rect(sf::Vector2f(CELLSIZE,CELLSIZE));
    bool found = false;
    for (const auto& row : *(board->getBoard())) {// перебираем все фигуры
        for (const auto& fig : row) {
            if (fig) {
                if (fig->getFigureType() == figure::KING and  fig->getTeam() == team){ // если король нужного цвета
                    rect.setPosition(fig->getPos().first*CELLSIZE+OFFSETX, (7-fig->getPos().second)*CELLSIZE+OFFSETY); // ставим квадрат
                    rect.setFillColor(sf::Color(100, 0, 0, 180));
                    found = true;
                    break;
                }
            }
        }
        if (found)
            break;
    }
    window.draw(rect); // рисуем квадрат

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

    for (int i = 0; i < types.size(); i++) {
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

// выбрать и превратить фигуру
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


void drawCastleButtons(sf::RenderWindow& window,sf::RectangleShape& rightCastle, sf::RectangleShape& leftCastle,sf::Font& font){// отрисовать кнопки рокировки для фишера
    rightCastle = makeButton(50, 50, sf::Color(177, 199, 90), sf::Color::Black); // создаем кнопку
    rightCastle.setPosition(875, window.getSize().y/2+25); // ставим на позицию
    window.draw(rightCastle); // отрисовываем
    drawLabel(window, font, rightCastle, "right\ncastle", 18); // и отрисовываем текст на ней

    leftCastle = makeButton(50, 50, sf::Color(177, 199, 90), sf::Color::Black); // создаем кнопку
    leftCastle.setPosition(875, window.getSize().y/2-25); // ставим на позицию
    window.draw(leftCastle); // отрисовываем
    drawLabel(window, font, leftCastle, "left\ncastle", 18); // и отрисовываем текст на ней
} 

void drawEndGameScreen(sf::RenderWindow& window,  // отрисовать экран конца игры
                       figure::teams winner,
                       const sf::Font& font,
                       sf::RectangleShape& btnRect)
{
    sf::Text winText(winner == figure::WHITE ? "WHITE WINS" : // текст
                     (winner == figure::BLACK ? "BLACK WINS" : "DRAW"),
                     font, 64);
    winText.setFillColor(sf::Color::White);
    auto winBounds = winText.getLocalBounds();
    winText.setOrigin(winBounds.width / 2.f, winBounds.height / 2.f);
    winText.setPosition(window.getSize().x / 2.f,
                        window.getSize().y / 2.f - 120.f);
    window.draw(winText);

    const sf::Vector2f winSize(window.getSize()); // кнопка
    btnRect = makeButton(300.f, 80.f,
                         sf::Color(180, 180, 180),  // фон
                         sf::Color::Black);         // обводка
    btnRect.setPosition(winSize.x / 2.f,
                        winSize.y / 2.f + 20.f);
    window.draw(btnRect);

    drawLabel(window, font, btnRect, "Restart Game", 32); // текст на кнопке
}


static void drawBackButton(sf::RenderWindow& win, sf::RectangleShape& backBtn, const sf::Font& font) { // создание кнопки back почти для каждого раздела меню
    const float W = 100, H = 40; 
    backBtn = makeButton(W, H, sf::Color(220, 100, 100), sf::Color::Black); // создаем кнопку
    backBtn.setPosition(win.getSize().x - W/2 - 10, win.getSize().y - H/2 - 10); // ставим на позицию
    win.draw(backBtn); // отрисовываем
    drawLabel(win, font, backBtn, "Back", 18); // и отрисовываем текст на ней
}

void drawMainMenu(sf::RenderWindow& win, std::map<std::string, sf::RectangleShape>& btns, sf::Font& font) { // отрисовка главного меню игры
    btns.clear();
    const float W = 300, H = 60, M = 20; // параметры кнопок
    auto center = sf::Vector2f(win.getSize()) * 0.5f;
    std::vector<std::pair<std::string, std::string>> list = { // список кнопок и их id
        {"play",    "Play"},
        {"puzzles", "Puzzles"},
        {"exit",    "Exit"}
    };
    float total = list.size()*H + (list.size()-1)*M; // сколько по высоте будут занимать кнопки
    float startY = center.y - total/2 + H/2; // с какого y начинать рисовать кнопки
    for (int i=0; i<list.size(); i++) { // по всему списку проходимся
        auto [key, label] = list[i]; 
        auto btn = makeButton(W, H, sf::Color(100,149,237), sf::Color::Black); // создаем кнопку
        btn.setPosition(center.x, startY + i*(H+M)); // ставим позицию
        btns[key] = btn;
        win.draw(btn); 
        drawLabel(win, font, btn, label); // рисуем текст на ней
    }
}

void drawPuzzleMenu(sf::RenderWindow& win, // отрисовка меню выбора задач #TODO сделать чтобы вообще эти задачи хоть как то работали
                    std::map<std::string, sf::RectangleShape>& numberBtns,
                    sf::RectangleShape& backBtn,
                    sf::RectangleShape& createBtn,
                    sf::Font& font,
                    int puzzleCount)
{
    numberBtns.clear();
    const float BTN = 50, SP = 10; // размер кнопок и расстояние между ними
    const int cols = 14; // количесво колонок с кнопками 14 - максимум что влезает в экран с текущими параметрами
    sf::FloatRect area(30, 20, win.getSize().x-20, win.getSize().y-25); // создаем область для кнопок
    float y0 = area.top;
    for (int idx=0; idx<puzzleCount; idx++) { // размещаем кнопки
        int r = idx/cols, c = idx%cols ; // строка и колонка для кнопки
        auto btn = makeButton(BTN, BTN, sf::Color(200,200,200), sf::Color::Black); // создаем кнопку
        btn.setPosition(area.left + c*(BTN+SP) + BTN/2,
                        y0 + r*(BTN+SP) + BTN/2);
        numberBtns[std::to_string(idx+1)] = btn; // записываем эту кнопку в массив
        win.draw(btn);
        drawLabel(win, font, btn, std::to_string(idx+1), 20); // рисуем надпись на кнопке
    }
    const float CW = 700, CH = 40; // параметры кнопки создать ширина и высота
    createBtn = makeButton(CW, CH, sf::Color(100,220,100), sf::Color::Black); // кнопка
    createBtn.setPosition(area.left + CW/2, win.getSize().y - CH/2 - 10);
    win.draw(createBtn);
    drawLabel(win, font, createBtn, "Create", 18); // текст
    drawBackButton(win, backBtn, font); // создаем кнопку back
}

void drawGameTypeMenu(sf::RenderWindow& win, // отрисовка меню выбора режима игры класический фишер до 3-х шахов
                      std::map<std::string, sf::RectangleShape>& btns,
                      sf::RectangleShape& backBtn,
                      sf::Font& font)
{
    btns.clear();
    const float W=280, H=60, M=20; // ширина высота расстояние между кнопками
    auto c = sf::Vector2f(win.getSize())*0.5f; // получаем размер окна /2 (точка центра окна)
    std::vector<std::pair<std::string,std::string>> list = {  // список кнопок и их id
        {"classic","Classic"},
        {"fischer","Fischer"},
        {"three","3-Check"}
    };
    float total = list.size()*H + (list.size()-1)*M; // сколько всего по y занимают кнопки
    float y0 = c.y - total/2 + H/2; // точка начала создания кнопок по y
    for (int i=0;i<list.size();i++) { 
        auto [k,l] = list[i];
        auto b = makeButton(W,H,sf::Color(255,215,0),sf::Color::Black); // создаем кнопки
        b.setPosition(c.x, y0 + i*(H+M));
        btns[k]=b;
        win.draw(b);
        drawLabel(win,font,b,l); // рисуем текст на кнопках
    }
    drawBackButton(win, backBtn, font); // рисуем кнопку назад (back)
}

void drawOpponentMenu(sf::RenderWindow& win, // отрисовка меню выбора противника ии или игрок
                      std::map<std::string, sf::RectangleShape>& btns,
                      sf::RectangleShape& backBtn,
                      sf::Font& font)
{
    btns.clear();
    const float W=220, H=50, M=15; // ширина высота расстояние между кнопками
    auto c = sf::Vector2f(win.getSize())*0.5f;  // получаем размер окна /2 (точка центра окна)
    std::vector<std::pair<std::string,std::string>> list = {
        {"pvp","Vs Player"}, // список кнопок и их id
        {"pve","Vs Computer"}
    };
    float total = list.size()*H + (list.size()-1)*M; // сколько всего по y занимают кнопки
    float y0 = c.y - total/2 + H/2; // точка начала создания кнопок по y
    for (int i=0;i<list.size();i++) {
        auto [k,l] = list[i];
        auto b = makeButton(W,H,sf::Color(173,216,230),sf::Color::Black); // создаем кнопки
        b.setPosition(c.x, y0 + i*(H+M));
        btns[k]=b;
        win.draw(b);
        drawLabel(win,font,b,l); // рисуем текст на кнопках
    }
    drawBackButton(win, backBtn, font);// рисуем кнопку назад (back)
}

void drawColorMenu(sf::RenderWindow& win, // отрисовка меню выбора команды белые или черные
                   std::map<std::string, sf::RectangleShape>& btns,
                   sf::RectangleShape& backBtn,
                   sf::Font& font)
{
    btns.clear();
    const float W=180, H=50, M=20; // ширина высота расстояние между кнопками
    auto c = sf::Vector2f(win.getSize())*0.5f;  // получаем размер окна /2 (точка центра окна)
    std::vector<std::pair<std::string,std::string>> list = {
        {"white","White"}, // список кнопок и их id
        {"black","Black"}
    };
    float total = list.size()*H + (list.size()-1)*M; // сколько всего по y занимают кнопки
    float y0 = c.y - total/2 + H/2; // точка начала создания кнопок по y
    for (int i=0;i<list.size();i++) {
        auto [k,l] = list[i];
        auto b = makeButton(W,H,sf::Color(240,240,240),sf::Color::Black); // создаем кнопки
        b.setPosition(c.x, y0 + i*(H+M));
        btns[k]=b;
        win.draw(b);
        drawLabel(win,font,b,l); // рисуем текст на кнопках
    }
    drawBackButton(win, backBtn, font); // рисуем кнопку назад (back)
} 
