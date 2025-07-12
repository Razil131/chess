#include <SFML/Graphics.hpp>
#include "game.hpp"
#include "board.hpp"
#include "engine.hpp"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(900, 900), "Chess", sf::Style::Titlebar | sf::Style::Close); // создаем окошечно так сказать 900 на 900 пикселей с название Chess

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

    //цифры
    sf::Text numbers[8];
    createNumbers(numbers, font, CELLSIZE, OFFSETY);

    std::map<std::string, sf::Texture> textures; // мапа текстур
    loadTextures(textures);
    Board* board = new Board();  // создание твоей доски (❁´◡`❁)
    board->initialize(textures); // расставление фигур на твоей доске (╯°□°）╯︵ ┻━┻

    bool isFigureSelected = false; // фигура сейчас выбрана для хода
    figure* selectedFigure = nullptr; 
    std::vector<std::pair<int, int>> possibleMoves; // возможные ходы для выбранной фигуры

    bool endGameScreen=false; // должен ли быть экран завершения игры?
    sf::RectangleShape newGameButtonRect; // прямоугольник для начала новой игры

    std::vector<sf::Sprite> to_choose; // спрайты фигур в меню выбора
    std::vector<sf::RectangleShape> rectangles_to_choose; // прямоугольники на заднем плане в меню выбора

    sf::RectangleShape lastMoveFrom(sf::Vector2f(CELLSIZE, CELLSIZE)); // квадратик откуда последний ход
    sf::RectangleShape lastMoveTo(sf::Vector2f(CELLSIZE, CELLSIZE)); // квадратик куда последний ход
    lastMoveFrom.setFillColor(sf::Color(0, 255, 0, 80)); // цвет
    lastMoveTo.setFillColor(sf::Color(0, 255, 0, 80)); // цвет
    bool hasMoved = false; // флаг был ли уже ход а то при запуске когда хода не было сделано эти квадраты просто на угол уезжали и закрывали часть окна

    int mode = 1; //тут по идее мы делаем от одного до 3, где 1 - дефлот шахматы, 2 - фишер, 3 - до 3 шахов
    int player = 1; //если 1 - игра с компом, если 2 - два игрока
    int dif = 1; //TODO потом switch case сделать чтобы выбирать сложность
    figure::teams userTeam = figure::BLACK; //TODO тут тоже сделать надо чтобы выбирать команду, но пока так
    
    //TODO тут ты сделаешь какой нибудь выбор с кем играть там вся тема такая, потом режимы сделаем
    Engine engine;
    if(player == 1){
        const char* enginePath = "..\\stockfish\\stockfish-windows-x86-64-avx2.exe"; //траим запустить движок
        if (!engine.launch(enginePath)) {
        std::cerr << "ERROR: не удалось запустить движок по пути `" << enginePath << "`\n";
        delete board;
        return -1;
        }
        engine.sendCommand("uci"); //включаем протокол UCI 😎
        engine.sendCommand("isready"); //проверяем готовность
        //engine.sendCommand("position fen 4k3/8/8/8/8/8/PPPPPPPP/4K3 w - - 0 1");
    }
    
    while (window.isOpen()) { // основной цикл постоянно повторяется пока окно открыто
        processEvents(window, board, endGameScreen, newGameButtonRect, isFigureSelected, selectedFigure, possibleMoves, lastMoveFrom, lastMoveTo, textures, to_choose, rectangles_to_choose, hasMoved, OFFSETX, OFFSETY, CELLSIZE); // обрабатываем все возможные события клик мыши и тд

        if (player == 1 && board->getCurrentTeam() != userTeam) { //проверяем когда ходит бот
        std::string moves; 
        for (const auto& move : board->movesUCI) { //переводим ходы из наших координат в е2е4 условные
            moves += move + " ";
        }

        engine.sendCommand("position startpos moves " + moves); //отправляем позицию движку
        engine.sendCommand("go movetime 1000"); //даем подумать секунду, по идее тут можно поменять на диф

        std::string bestmove;//получаем лучший ход
        do {
            auto resp = engine.readLine();
            if (resp.find("bestmove") != std::string::npos) {
            size_t start = resp.find("bestmove") + 9;
            size_t end = resp.find(' ', start);
            bestmove = (end == std::string::npos) ? resp.substr(start) : resp.substr(start, end - start);
            }
        } while (bestmove.empty());

        if (bestmove == "resign") {
        //TODO бот сдаться может, тут надо че то сделать по этому поводу тебе
        }

        //парсим координаты
        int fx, fy, tx, ty;
    char prom = '\0';
    if (bestmove.length() >= 4) {
        fx = bestmove[0] - 'a';
        fy = bestmove[1] - '1';
        tx = bestmove[2] - 'a';
        ty = bestmove[3] - '1';
        
        if (bestmove.length() >= 5) {// символ превращения
            prom = bestmove[4];
            }
        } 
        
        board->makeMove({fx, fy}, {tx, ty});//выполняем ход

        
        if (prom != '\0') {
        figure::figureTypes newType;
        switch (prom) {
            case 'r': newType = figure::ROOK; break;
            case 'b': newType = figure::BISHOP; break;
            case 'n': newType = figure::KNIGHT; break;
            case 'q': 
            default: newType = figure::QUEEN;
        }

        std::string teamChar = (userTeam == figure::WHITE) ? "b" : "w"; //определяем команду бота
        std::string textureName;
        switch (newType) {
            case figure::QUEEN: textureName = "q" + teamChar; break;
            case figure::ROOK: textureName = "r" + teamChar; break;
            case figure::BISHOP: textureName = "b" + teamChar; break;
            case figure::KNIGHT: textureName = "n" + teamChar; break;
            default: textureName = "q" + teamChar;
        }

        board->convertPawn(tx, ty, newType, textures[textureName]); //превращаем
        

        board->convertFlag = false;
    }
}

        window.clear(sf::Color(128,128,128)); // отчищаем окно чтобы оно обновлялось цвет в скобках это цвет фона (серый)

        drawBoardAndLabels(window, boardRectangles, letters, numbers); // рисуем доску и цифры буквы
        if (hasMoved) { // если ход был рисуем зеленые квадраты на последнем ходу
            window.draw(lastMoveFrom);
            window.draw(lastMoveTo);
        }
        if (board->isKingInCheck(figure::BLACK)){ // если какому нибудь королю стоит шах нарисовать красный квадрат на нем
            drawCheck(window,board,figure::BLACK,OFFSETX,OFFSETY,CELLSIZE);
        }else if(board->isKingInCheck(figure::WHITE)){
            drawCheck(window,board,figure::WHITE,OFFSETX,OFFSETY,CELLSIZE);
        }

        drawFigures(window, board, CELLSIZE, OFFSETX, OFFSETY); // рисуем фигуры

        if (board->isKingInMate(figure::WHITE)){ // если у кого то мат или пат рисуем экран конца игры
            drawEndGameScreen(window,figure::BLACK,font, newGameButtonRect); 
            endGameScreen=true;
        }else if(board->isKingInMate(figure::BLACK)){
            drawEndGameScreen(window,figure::WHITE,font, newGameButtonRect);
            endGameScreen=true;
        }else if (board->isKingInStalemate(figure::WHITE) or board->isKingInStalemate(figure::BLACK)){
            drawEndGameScreen(window,figure::NONE,font, newGameButtonRect);
            endGameScreen=true;
        }

        if (isFigureSelected) { // если фигура выбрана
            drawMoveHighlights(window, possibleMoves, *board, selectedFigure, OFFSETX, OFFSETY, CELLSIZE); // рисуем возможные ходы
        }

        if (board->convertFlag && board->getCurrentTeam() == userTeam){ // если пешка на клетке для превращения
            createChoiceMenu(board, to_choose, rectangles_to_choose, textures, OFFSETX, OFFSETY, CELLSIZE); // создаем и отрисовываем меню выбора
            drawChoiceMenu(window, to_choose, rectangles_to_choose);
        }

        window.display(); // показывалось окно чтобы
        
    }
    
    engine.stop();
    delete board; // отчищаем память от твоей доски 😥😥😣😣😥
    return 0;
}