#include "modes.hpp"

void vsPlayerStandart(sf::RenderWindow& window,sf::Font& font, std::string savefile){    // отдельная функция для игры против игрока
    const float CELLSIZE = 100.f; // размер клетки
    const float OFFSETX = 50.f; // отстпуп для букв слева
    const float OFFSETY = 50.f; // отступ для цифр снизу
    
    //буквы
    sf::Text letters[8];
    createLetters(letters, font, CELLSIZE, OFFSETX, OFFSETY);

    //цифры
    sf::Text numbers[8];
    createNumbers(numbers, font, CELLSIZE, OFFSETY);

    std::map<std::string, sf::Texture> textures; // мапа текстур
    loadTextures(textures);
    Board* board = new Board();  // создание твоей доски (❁´◡`❁)
    if (savefile.empty())
        board->initialize(textures); // расставление фигур на твоей доске (╯°□°）╯︵ ┻━┻
    else
        board->importFromFile(savefile,textures);

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

    // создаем доску
    sf::RectangleShape boardRectangles[8][8];
    initializeBoardRectangles(boardRectangles, CELLSIZE, OFFSETX, OFFSETY);

    while (window.isOpen()){//             мод человек сколько и команда
        processEvents(window, font, board, 1, 2, endGameScreen, newGameButtonRect, isFigureSelected, selectedFigure, possibleMoves, lastMoveFrom, lastMoveTo, textures, to_choose, rectangles_to_choose, hasMoved, OFFSETX, OFFSETY, CELLSIZE); // обрабатываем все возможные события клик мыши и тд

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

        if (board->convertFlag){ // если пешка на клетке для превращения
            createChoiceMenu(board, to_choose, rectangles_to_choose, textures, OFFSETX, OFFSETY, CELLSIZE); // создаем и отрисовываем меню выбора
            drawChoiceMenu(window, to_choose, rectangles_to_choose);
        }

        window.display(); // показывалось окно чтобы
    }
    delete board; // отчищаем память от твоей доски 😥😥😣😣😥
}

void vsPlayerFisher(sf::RenderWindow& window,sf::Font& font, std::string savefile){    // отдельная функция для игры против игрока
    const float CELLSIZE = 100.f; // размер клетки
    const float OFFSETX = 50.f; // отстпуп для букв слева
    const float OFFSETY = 50.f; // отступ для цифр снизу
    
    //буквы
    sf::Text letters[8];
    createLetters(letters, font, CELLSIZE, OFFSETX, OFFSETY);

    //цифры
    sf::Text numbers[8];
    createNumbers(numbers, font, CELLSIZE, OFFSETY);

    std::map<std::string, sf::Texture> textures; // мапа текстур
    loadTextures(textures);
    Board* board = new Board();  // создание твоей доски (❁´◡`❁)
    if (savefile.empty())
        board->fisherPos(textures); // расставление фигур на твоей доске (╯°□°）╯︵ ┻━┻
    else
        board->importFromFile(savefile,textures); // расставление фигур на твоей доске (╯°□°）╯︵ ┻━┻

    bool isFigureSelected = false; // фигура сейчас выбрана для хода
    figure* selectedFigure = nullptr; 
    std::vector<std::pair<int, int>> possibleMoves; // возможные ходы для выбранной фигуры

    bool endGameScreen=false; // должен ли быть экран завершения игры?
    sf::RectangleShape newGameButtonRect; // прямоугольник для начала новой игры

    std::vector<sf::Sprite> to_choose; // спрайты фигур в меню выбора
    std::vector<sf::RectangleShape> rectangles_to_choose; // прямоугольники на заднем плане в меню выбора

    sf::RectangleShape rightCastle;
    sf::RectangleShape leftCastle;

    sf::RectangleShape lastMoveFrom(sf::Vector2f(CELLSIZE, CELLSIZE)); // квадратик откуда последний ход
    sf::RectangleShape lastMoveTo(sf::Vector2f(CELLSIZE, CELLSIZE)); // квадратик куда последний ход
    lastMoveFrom.setFillColor(sf::Color(0, 255, 0, 80)); // цвет
    lastMoveTo.setFillColor(sf::Color(0, 255, 0, 80)); // цвет
    bool hasMoved = false; // флаг был ли уже ход а то при запуске когда хода не было сделано эти квадраты просто на угол уезжали и закрывали часть окна

    // создаем доску
    sf::RectangleShape boardRectangles[8][8];
    initializeBoardRectangles(boardRectangles, CELLSIZE, OFFSETX, OFFSETY);

    while (window.isOpen()){
        bool handled = false;
        processEvents(window, font, board, 2, 2, endGameScreen, newGameButtonRect, isFigureSelected, selectedFigure, possibleMoves, lastMoveFrom, lastMoveTo, textures, to_choose, rectangles_to_choose, hasMoved, OFFSETX, OFFSETY, CELLSIZE, &rightCastle, &leftCastle); // обрабатываем все возможные события клик мыши и тд

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

        if (board->convertFlag){ // если пешка на клетке для превращения
            createChoiceMenu(board, to_choose, rectangles_to_choose, textures, OFFSETX, OFFSETY, CELLSIZE); // создаем и отрисовываем меню выбора
            drawChoiceMenu(window, to_choose, rectangles_to_choose);
        }
        drawCastleButtons(window,rightCastle,leftCastle,font);
        window.display(); // показывалось окно чтобы
    }
    delete board; // отчищаем память от твоей доски 😥😥😣😣😥
}

void vsComputerStandart(sf::RenderWindow& window,sf::Font& font, int difficult, figure::teams userTeam, std::string savefile){// отдельная функция для игры против компьютера
    const float CELLSIZE = 100.f; // размер клетки
    const float OFFSETX = 50.f; // отстпуп для букв слева
    const float OFFSETY = 50.f; // отступ для цифр снизу
    
    //буквы
    sf::Text letters[8];
    createLetters(letters, font, CELLSIZE, OFFSETX, OFFSETY);

    //цифры
    sf::Text numbers[8];
    createNumbers(numbers, font, CELLSIZE, OFFSETY);

    std::map<std::string, sf::Texture> textures; // мапа текстур
    loadTextures(textures);
    Board* board = new Board();  // создание твоей доски (❁´◡`❁)
    if (savefile.empty())
        board->initialize(textures); // расставление фигур на твоей доске (╯°□°）╯︵ ┻━┻
    else
        board->importFromFile(savefile,textures);

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
  
    // создаем доску
    sf::RectangleShape boardRectangles[8][8];
    initializeBoardRectangles(boardRectangles, CELLSIZE, OFFSETX, OFFSETY);
  
    Engine engine;
    const char* enginePath = "..\\stockfish\\stockfish-windows-x86-64-avx2.exe"; //траим запустить движок
    if (!engine.launch(enginePath)) {
        std::cerr << "ERROR: не удалось запустить движок по пути `" << enginePath << "`\n";
        delete board;
        return;
    }
    engine.sendCommand("uci"); //включаем протокол UCI 😎
    std::string out = "setoption name Skill Level value " + std::to_string(difficult);
    engine.sendCommand(out);
    engine.sendCommand("isready"); //проверяем готовность
    std::string time = "go movetime " + std::to_string(difficult * 200);
    while (window.isOpen()) { // основной цикл постоянно повторяется пока окно открыто
        if (!endGameScreen){
            if (board->getCurrentTeam() != userTeam) { //проверяем когда ходит бот
                std::string moves; 
                for (const auto& move : board->movesUCI) { //переводим ходы из наших координат в е2е4 условные
                    moves += move + " ";
                }
                engine.sendCommand( savefile.empty() ? ("position startpos moves " + moves): (std::string("position fen ") + board->fenPos)); //отправляем позицию движку); //отправляем позицию движку
                engine.sendCommand(time); //даем подумать секунду, по идее тут можно поменять на диф

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
                    endGameScreen=true; // чет написал когда сдается но хз как проверить работает ли но вроде должно
                    drawEndGameScreen(window,userTeam, font, newGameButtonRect);
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
                    
                    if (board->makeMove({fx, fy}, {tx, ty})){//выполняем ход
                        lastMoveFrom.setPosition(
                        OFFSETX + fx * CELLSIZE,
                        OFFSETY + (7 - fy) * CELLSIZE
                        );
                        lastMoveTo.setPosition(
                            OFFSETX + tx * CELLSIZE,
                            OFFSETY + (7 - ty) * CELLSIZE
                        );
                    }

                    
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
        }
        processEvents(window, font, board, 1, 1, endGameScreen, newGameButtonRect, isFigureSelected, selectedFigure, possibleMoves, lastMoveFrom, lastMoveTo, textures, to_choose, rectangles_to_choose, hasMoved, OFFSETX, OFFSETY, CELLSIZE); // обрабатываем все возможные события клик мыши и тд

        

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
    delete board; // отчищаем память от твоей доски 😥😥😣😣😥
    engine.stop();
}

void vsComputerFisher(sf::RenderWindow& window,sf::Font& font, int difficult, figure::teams userTeam, std::string savefile){// отдельная функция для игры против компьютера
    const float CELLSIZE = 100.f; // размер клетки
    const float OFFSETX = 50.f; // отстпуп для букв слева
    const float OFFSETY = 50.f; // отступ для цифр снизу
    
    //буквы
    sf::Text letters[8];
    createLetters(letters, font, CELLSIZE, OFFSETX, OFFSETY);

    //цифры
    sf::Text numbers[8];
    createNumbers(numbers, font, CELLSIZE, OFFSETY);

    std::map<std::string, sf::Texture> textures; // мапа текстур
    loadTextures(textures);
    Board* board = new Board();  // создание твоей доски (❁´◡`❁)
    if (savefile.empty())
        board->fisherPos(textures); // расставление фигур на твоей доске (╯°□°）╯︵ ┻━┻
    else
        board->importFromFile(savefile,textures); // расставление фигур на твоей доске (╯°□°）╯︵ ┻━┻

    bool isFigureSelected = false; // фигура сейчас выбрана для хода
    figure* selectedFigure = nullptr; 
    std::vector<std::pair<int, int>> possibleMoves; // возможные ходы для выбранной фигуры

    bool endGameScreen=false; // должен ли быть экран завершения игры?
    sf::RectangleShape newGameButtonRect; // прямоугольник для начала новой игры

    std::vector<sf::Sprite> to_choose; // спрайты фигур в меню выбора
    std::vector<sf::RectangleShape> rectangles_to_choose; // прямоугольники на заднем плане в меню выбора

    sf::RectangleShape rightCastle;
    sf::RectangleShape leftCastle;

    sf::RectangleShape lastMoveFrom(sf::Vector2f(CELLSIZE, CELLSIZE)); // квадратик откуда последний ход
    sf::RectangleShape lastMoveTo(sf::Vector2f(CELLSIZE, CELLSIZE)); // квадратик куда последний ход
    lastMoveFrom.setFillColor(sf::Color(0, 255, 0, 80)); // цвет
    lastMoveTo.setFillColor(sf::Color(0, 255, 0, 80)); // цвет
    bool hasMoved = false; // флаг был ли уже ход а то при запуске когда хода не было сделано эти квадраты просто на угол уезжали и закрывали часть окна
  
    // создаем доску
    sf::RectangleShape boardRectangles[8][8];
    initializeBoardRectangles(boardRectangles, CELLSIZE, OFFSETX, OFFSETY);
  
    Engine engine;
    const char* enginePath = "..\\stockfish\\stockfish-windows-x86-64-avx2.exe"; //траим запустить движок
    if (!engine.launch(enginePath)) {
        std::cerr << "ERROR: не удалось запустить движок по пути `" << enginePath << "`\n";
        delete board;
        return;
    }
    engine.sendCommand("uci"); //включаем протокол UCI 😎
    engine.sendCommand("setoption name UCI_Chess960 value true"); //запускаем фишера
    std::string out = "setoption name Skill Level value " + std::to_string(difficult);
    engine.sendCommand(out);
    engine.sendCommand("isready"); //проверяем готовность
    std::string time = "go movetime " + std::to_string(difficult * 200);
    engine.sendCommand(std::string("position fen ") + board->fenPos); //сообщаем позицию боту


    while (window.isOpen()) { // основной цикл постоянно повторяется пока окно открыто
        
        processEvents(window, font, board, 2, 1, endGameScreen, newGameButtonRect, isFigureSelected, selectedFigure, possibleMoves, lastMoveFrom, lastMoveTo, textures, to_choose, rectangles_to_choose, hasMoved, OFFSETX, OFFSETY, CELLSIZE, &rightCastle, &leftCastle); // обрабатываем все возможные события клик мыши и тд
        if (!endGameScreen){
            if (board->getCurrentTeam() != userTeam) { //формируем строку всех предыдущих ходов
            std::string moves;
            for (const auto& m : board->movesUCI) {
                moves += m + " ";
            }
            engine.sendCommand("position fen " + board->fenPos + " moves " + moves); //отправляем движку позицию и ждём ход
            engine.sendCommand(time);

            std::string bestmove;
            do {
                auto resp = engine.readLine();
                auto p = resp.find("bestmove");
                if (p != std::string::npos) {
                    size_t s = p + 9, e = resp.find(' ', s);
                    bestmove = (e == std::string::npos)
                            ? resp.substr(s)
                            : resp.substr(s, e - s);
                }
            } while (bestmove.empty());

            //разбираем строку на наши коорды
            int fx = bestmove[0] - 'a';
            int fy = bestmove[1] - '1';
            int tx = bestmove[2] - 'a';
            int ty = bestmove[3] - '1';
            char prom = bestmove.size() >= 5 ? bestmove[4] : '\0';

            //делаем ход
            board->makeMove({fx, fy}, {tx, ty});
            lastMoveFrom.setPosition(OFFSETX + fx * CELLSIZE,
                                    OFFSETY + (7 - fy) * CELLSIZE);
            lastMoveTo  .setPosition(OFFSETX + tx * CELLSIZE,
                                    OFFSETY + (7 - ty) * CELLSIZE);
            hasMoved = true;

            //превращение
            if (prom) {
                figure::figureTypes newType;
                switch (prom) {
                    case 'r': newType = figure::ROOK;   break;
                    case 'b': newType = figure::BISHOP; break;
                    case 'n': newType = figure::KNIGHT; break;
                    case 'q': default:  newType = figure::QUEEN;
                }
                std::string tname = (userTeam == figure::WHITE ? "b" : "w");
                switch (newType) {
                    case figure::QUEEN:  tname = "q" + tname; break;
                    case figure::ROOK:   tname = "r" + tname; break;
                    case figure::BISHOP: tname = "b" + tname; break;
                    case figure::KNIGHT: tname = "n" + tname; break;
                    default: break;
                }
                board->convertPawn(tx, ty, newType, textures[tname]);
            }      
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
        drawCastleButtons(window,rightCastle,leftCastle,font);
        window.display(); // показывалось окно чтобы
        
    }
    delete board; // отчищаем память от твоей доски 😥😥😣😣😥
    engine.stop();
}

void vsPlayer3Check(sf::RenderWindow& window,sf::Font& font, std::string savefile){    // отдельная функция для игры против игрока
    const float CELLSIZE = 100.f; // размер клетки
    const float OFFSETX = 50.f; // отстпуп для букв слева
    const float OFFSETY = 50.f; // отступ для цифр снизу
    
    //буквы
    sf::Text letters[8];
    createLetters(letters, font, CELLSIZE, OFFSETX, OFFSETY);

    //цифры
    sf::Text numbers[8];
    createNumbers(numbers, font, CELLSIZE, OFFSETY);

    std::map<std::string, sf::Texture> textures; // мапа текстур
    loadTextures(textures);
    
    int whiteChecks = 0;
    int blackChecks = 0;
    bool whiteWasInCheck = false;
    bool blackWasInCheck = false;

    Board* board = new Board();  // создание твоей доски (❁´◡`❁)
    if (savefile.empty())
        board->initialize(textures); // расставление фигур на твоей доске (╯°□°）╯︵ ┻━┻
    else
        board->importFromFile(savefile,textures);

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

    // создаем доску
    sf::RectangleShape boardRectangles[8][8];
    initializeBoardRectangles(boardRectangles, CELLSIZE, OFFSETX, OFFSETY);

    while (window.isOpen()){
        processEvents(window, font, board, 3, 2, endGameScreen, newGameButtonRect, isFigureSelected, selectedFigure, possibleMoves, lastMoveFrom, lastMoveTo, textures, to_choose, rectangles_to_choose, hasMoved, OFFSETX, OFFSETY, CELLSIZE); // обрабатываем все возможные события клик мыши и тд

        window.clear(sf::Color(128,128,128)); // отчищаем окно чтобы оно обновлялось цвет в скобках это цвет фона (серый)

        drawBoardAndLabels(window, boardRectangles, letters, numbers); // рисуем доску и цифры буквы
        if (hasMoved) { // если ход был рисуем зеленые квадраты на последнем ходу
            window.draw(lastMoveFrom);
            window.draw(lastMoveTo);
        }
        if (board->isKingInCheck(figure::BLACK)){ // если какому нибудь королю стоит шах нарисовать красный квадрат на нем
            if (!blackWasInCheck)
                blackChecks++;
            blackWasInCheck = true;
            drawCheck(window,board,figure::BLACK,OFFSETX,OFFSETY,CELLSIZE);
        }else if(board->isKingInCheck(figure::WHITE)){
            if (!whiteWasInCheck)
                whiteChecks++;
            whiteWasInCheck = true;
            drawCheck(window,board,figure::WHITE,OFFSETX,OFFSETY,CELLSIZE);
        }
        else{
            whiteWasInCheck=false;
            blackWasInCheck=false;
        }

        drawFigures(window, board, CELLSIZE, OFFSETX, OFFSETY); // рисуем фигуры

        if (board->isKingInMate(figure::BLACK) or blackChecks>=3){ // если у кого то мат или пат рисуем экран конца игры
            drawEndGameScreen(window,figure::WHITE,font, newGameButtonRect); 
            endGameScreen=true;
        }else if(board->isKingInMate(figure::WHITE) or whiteChecks>=3){
            drawEndGameScreen(window,figure::BLACK,font, newGameButtonRect);
            endGameScreen=true;
        }else if (board->isKingInStalemate(figure::WHITE) or board->isKingInStalemate(figure::BLACK)){
            drawEndGameScreen(window,figure::NONE,font, newGameButtonRect);
            endGameScreen=true;
        }

        if (isFigureSelected) { // если фигура выбрана
            drawMoveHighlights(window, possibleMoves, *board, selectedFigure, OFFSETX, OFFSETY, CELLSIZE); // рисуем возможные ходы
        }

        if (board->convertFlag){ // если пешка на клетке для превращения
            createChoiceMenu(board, to_choose, rectangles_to_choose, textures, OFFSETX, OFFSETY, CELLSIZE); // создаем и отрисовываем меню выбора
            drawChoiceMenu(window, to_choose, rectangles_to_choose);
        }

        window.display(); // показывалось окно чтобы
    }
    delete board; // отчищаем память от твоей доски 😥😥😣😣😥
}

void vsComputer3Check(sf::RenderWindow& window,sf::Font& font, int difficult, figure::teams userTeam, std::string savefile){// отдельная функция для игры против компьютера
    const float CELLSIZE = 100.f; // размер клетки
    const float OFFSETX = 50.f; // отстпуп для букв слева
    const float OFFSETY = 50.f; // отступ для цифр снизу
    
    int whiteChecks = 0;
    int blackChecks = 0;
    bool whiteWasInCheck = false;
    bool blackWasInCheck = false;

    //буквы
    sf::Text letters[8];
    createLetters(letters, font, CELLSIZE, OFFSETX, OFFSETY);

    //цифры
    sf::Text numbers[8];
    createNumbers(numbers, font, CELLSIZE, OFFSETY);

    std::map<std::string, sf::Texture> textures; // мапа текстур
    loadTextures(textures);
    Board* board = new Board();  // создание твоей доски (❁´◡`❁)
    if (savefile.empty())
        board->initialize(textures); // расставление фигур на твоей доске (╯°□°）╯︵ ┻━┻
    else
        board->importFromFile(savefile,textures);

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
  
    // создаем доску
    sf::RectangleShape boardRectangles[8][8];
    initializeBoardRectangles(boardRectangles, CELLSIZE, OFFSETX, OFFSETY);
  
    Engine engine;
    const char* enginePath = "..\\stockfish\\stockfish-windows-x86-64-avx2.exe"; //траим запустить движок
    if (!engine.launch(enginePath)) {
        std::cerr << "ERROR: не удалось запустить движок по пути `" << enginePath << "`\n";
        delete board;
        return;
    }
    engine.sendCommand("uci"); //включаем протокол UCI 😎
    std::string out = "setoption name Skill Level value " + std::to_string(difficult);
    engine.sendCommand(out);
    engine.sendCommand("isready"); //проверяем готовность
    std::string time = "go movetime " + std::to_string(difficult * 200);
    while (window.isOpen()) { // основной цикл постоянно повторяется пока окно открыто
        if (!endGameScreen){
            if (board->getCurrentTeam() != userTeam) { //проверяем когда ходит бот
                std::string moves; 
                for (const auto& move : board->movesUCI) { //переводим ходы из наших координат в е2е4 условные
                    moves += move + " ";
                }

                engine.sendCommand( savefile.empty() ? ("position startpos moves " + moves): (std::string("position fen ") + board->fenPos));
                engine.sendCommand(time); //даем подумать секунду

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
                    endGameScreen=true; // чет написал когда сдается но хз как проверить работает ли но вроде должно
                    drawEndGameScreen(window,userTeam, font, newGameButtonRect);
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
                    
                    if (board->makeMove({fx, fy}, {tx, ty})){//выполняем ход
                        lastMoveFrom.setPosition(
                        OFFSETX + fx * CELLSIZE,
                        OFFSETY + (7 - fy) * CELLSIZE
                        );
                        lastMoveTo.setPosition(
                            OFFSETX + tx * CELLSIZE,
                            OFFSETY + (7 - ty) * CELLSIZE
                        );
                    }

                    
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
        }
        processEvents(window, font, board, 3, 1, endGameScreen, newGameButtonRect, isFigureSelected, selectedFigure, possibleMoves, lastMoveFrom, lastMoveTo, textures, to_choose, rectangles_to_choose, hasMoved, OFFSETX, OFFSETY, CELLSIZE); // обрабатываем все возможные события клик мыши и тд

        

        window.clear(sf::Color(128,128,128)); // отчищаем окно чтобы оно обновлялось цвет в скобках это цвет фона (серый)

        drawBoardAndLabels(window, boardRectangles, letters, numbers); // рисуем доску и цифры буквы
        if (hasMoved) { // если ход был рисуем зеленые квадраты на последнем ходу
            window.draw(lastMoveFrom);
            window.draw(lastMoveTo);
        }
        if (board->isKingInCheck(figure::BLACK)){ // если какому нибудь королю стоит шах нарисовать красный квадрат на нем
            if (!blackWasInCheck)
                blackChecks++;
            blackWasInCheck = true;
            drawCheck(window,board,figure::BLACK,OFFSETX,OFFSETY,CELLSIZE);
        }else if(board->isKingInCheck(figure::WHITE)){
            if (!whiteWasInCheck)
                whiteChecks++;
            whiteWasInCheck = true;
            drawCheck(window,board,figure::WHITE,OFFSETX,OFFSETY,CELLSIZE);
        }else{
            whiteWasInCheck=false;
            blackWasInCheck=false;
        }

        drawFigures(window, board, CELLSIZE, OFFSETX, OFFSETY); // рисуем фигуры

        

        if (board->isKingInMate(figure::WHITE) or blackChecks>=3){ // если у кого то мат или пат рисуем экран конца игры
            drawEndGameScreen(window,figure::BLACK,font, newGameButtonRect); 
            endGameScreen=true;
        }else if(board->isKingInMate(figure::BLACK) or whiteChecks>=3){
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
    delete board; // отчищаем память от твоей доски 😥😥😣😣😥
    engine.stop();
}

// открыть меню создания задачи
void createPuzzle(sf::RenderWindow& window,sf::Font& font, std::string puzzleNum){
    const float CELLSIZE = 100.f; // размер клетки
    const float OFFSETX = 50.f; // отстпуп для букв слева
    const float OFFSETY = 50.f; // отступ для цифр снизу
    
    enum createPuzzleModes {Setup,Game}; // режими сначало расстановка фигур потом ходить надо(Game)

    createPuzzleModes currentMode = Setup;

    std::string newPuzzleFileName = generatePuzzleFilename();

    //буквы
    sf::Text letters[8];
    createLetters(letters, font, CELLSIZE, OFFSETX, OFFSETY);

    //цифры
    sf::Text numbers[8];
    createNumbers(numbers, font, CELLSIZE, OFFSETY);

    std::map<std::string, sf::Texture> textures; // мапа текстур
    loadTextures(textures);
    Board* board = new Board();  // создание твоей доски (❁´◡`❁)
    if (puzzleNum != "-1"){ // если редачим то загружаем из файла иначе отчищаем
        newPuzzleFileName = makePuzzleFilename(puzzleNum);
        board->loadFirstFenAndDeleteFile(newPuzzleFileName,textures);
    }
    else
        board->clear();

    std::pair<int,int> selectedCell = std::make_pair(-1,-1); // выбраная клетка на которую ставить будем фигуру
    bool cellIsSelected = false; 
    int choosedFigurePos; // позиция фигуры в меню выбора от 0 до 11

    std::vector<sf::Sprite> figureSpritesToChoose; // спрайты фигур в меню выбора
    std::vector<sf::RectangleShape> rectanglesToChoose; // прямоугольники на заднем плане в меню выбора
    sf::RectangleShape container; // прямоугольник в котором все кнопки в меню выбора

    sf::RectangleShape nextModeBtnShape; // кнопка перехода на след режим
    nextModeBtnShape = makeButton(25,700,sf::Color(0,255,0),sf::Color(0,0,0));
    nextModeBtnShape.setPosition(OFFSETX*1.5+CELLSIZE*8,OFFSETY+CELLSIZE*4);

    sf::RectangleShape choosedCellRect(sf::Vector2f(CELLSIZE, CELLSIZE)); // квадратик отображающий клетку выбраную
    choosedCellRect.setFillColor(sf::Color(0, 255, 0, 80)); // цвет
  
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

    // создаем доску
    sf::RectangleShape boardRectangles[8][8];
    initializeBoardRectangles(boardRectangles, CELLSIZE, OFFSETX, OFFSETY);
    while (window.isOpen()){
        window.clear(sf::Color(128,128,128));
        sf::Event event;
        if (currentMode == Setup){// если расстановка фигур
            while (window.pollEvent(event)) {
                handleWindowClose(window,event);
                if (event.type == sf::Event::MouseButtonPressed && // нажатие левой кнопки мыши
                event.mouseButton.button == sf::Mouse::Left){
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y); // получаем положение курсора
                    if (nextModeBtnShape.getGlobalBounds().contains(mousePos)){ // если нажали на кнопку далее переходи в след режим
                        board->logFen(newPuzzleFileName);
                        currentMode = Game;
                        continue;
                    }
                    if (cellIsSelected){ // если клетка поля уже выбрана
                        choosedFigurePos = handleCreatePuzzleEvents(mousePos,container,CELLSIZE); // позиция фигуры в меню выбора от 0 до 11
                        if (choosedFigurePos != -1) {
                            std::vector<std::string> types = {"k", "q", "b", "n", "r", "p"}; // Позиции: 0–5 — белые фигуры, 6–11 — черные
                            int typeIndex = choosedFigurePos % 6; // какая по счету фигура в строке (столбец)
                            bool isWhite = choosedFigurePos < 6; // белые ли
                            std::string figure = types[typeIndex] + (isWhite ? "w" : "b"); // создаем строку например qw q-queen w-white

                            char typeChar = figure[0];  // kqbnrp
                            char colorChar = figure[1]; // wb (wildberries)

                            figure::teams team = (colorChar == 'w') ? figure::teams::WHITE : figure::teams::BLACK;

                            if (typeChar == 'k') // ставим нужную фигуру на нужное место 
                                board->setFigure(selectedCell.first, selectedCell.second, std::make_unique<king>(team, selectedCell, textures[figure]));
                            else if (typeChar == 'q')
                                board->setFigure(selectedCell.first, selectedCell.second, std::make_unique<queen>(team, selectedCell, textures[figure]));
                            else if (typeChar == 'b')
                                board->setFigure(selectedCell.first, selectedCell.second, std::make_unique<bishop>(team, selectedCell, textures[figure]));
                            else if (typeChar == 'n')
                                board->setFigure(selectedCell.first, selectedCell.second, std::make_unique<knight>(team, selectedCell, textures[figure]));
                            else if (typeChar == 'r')
                                board->setFigure(selectedCell.first, selectedCell.second, std::make_unique<rook>(team, selectedCell, textures[figure]));
                            else if (typeChar == 'p')
                                board->setFigure(selectedCell.first, selectedCell.second, std::make_unique<pawn>(team, selectedCell, textures[figure]));

                            selectedCell = std::make_pair(-1,-1); // обнуляем выбранную ячейку
                            cellIsSelected = false;
                            continue;
                        }
                    }
                    
                    for (int x = 0; x<8; x++){ // если кликнули не по менб выбора то мб кликнули по клетке
                        for (int y = 0; y<8; y++){// по всем клеткам проходимся
                            if (boardRectangles[x][y].getGlobalBounds().contains(mousePos)){
                                selectedCell=std::make_pair(x,7-y); // ставим выбранную клетку
                                cellIsSelected = true;
                                choosedCellRect.setPosition(
                                OFFSETX + selectedCell.first * CELLSIZE,
                                OFFSETY + (7 - selectedCell.second) * CELLSIZE);
                                break;
                            }
                        }
                    }
                }
                else if (event.type == sf::Event::MouseButtonPressed && // нажатие правой кнопки мыши
                event.mouseButton.button == sf::Mouse::Right){
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y); // получаем положение курсора
                    for (int x = 0; x<8; x++){
                        for (int y = 0; y<8; y++){// по всем клеткам проходимся
                            if (boardRectangles[x][y].getGlobalBounds().contains(mousePos)){
                                board->removeFigure(x,7-y); // убираем фигуру на выбранной клетке
                                break;
                            }
                        }
                    }
                }
            }
            
            drawBoardAndLabels(window,boardRectangles,letters,numbers);
            if (cellIsSelected)
                window.draw(choosedCellRect);
            drawNextModeButton(window,nextModeBtnShape,font,CELLSIZE,OFFSETX,OFFSETY);
            drawFigures(window,board,CELLSIZE,OFFSETX,OFFSETY);
            if (selectedCell.first!=-1 and selectedCell.second!=-1){
                createChooseFigureMenuCreatePuzzle(window,textures,selectedCell,CELLSIZE,OFFSETX,OFFSETY,container,figureSpritesToChoose,rectanglesToChoose);
                drawChooseFigureMenuCreatePuzzle(window,container,figureSpritesToChoose,rectanglesToChoose);
            }
        }

        else if(currentMode == Game){ // если второй режим там где ходить надо
            sf::Event event;  // какое событие происходит сейчас клик мыши или закрытие окно
            while (window.pollEvent(event)) { // получаем постоянно событие какое то
                handleWindowClose(window, event); // чтобы закрывалось окно
                if (event.type == sf::Event::MouseButtonPressed && // нажатие левой кнопки мыши
                    event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y); // получаем положение курсора
                    // если пешка на клетке для превращения
                    if (nextModeBtnShape.getGlobalBounds().contains(mousePos)){
                        delete board;
                        createMainMenu(window,font);
                    }
                    if (board->convertFlag){ 
                        if (selectFigureToConvert(board, rectangles_to_choose, mousePos, textures, OFFSETX, CELLSIZE)) // выбираем и превращаем
                            board->logFen(newPuzzleFileName);// если превращение произошло то сохраняем позицию
                        else
                            continue;
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
                            if (!(board->convertFlag) or !(selectedFigure->getTeam() == figure::BLACK)) // если не было превращение или превращается черная фигура то сохраняем
                                board->logFen(newPuzzleFileName);
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

            if (isFigureSelected) { // если фигура выбрана
                drawMoveHighlights(window, possibleMoves, *board, selectedFigure, OFFSETX, OFFSETY, CELLSIZE); // рисуем возможные ходы
            }

            if (board->convertFlag){ // если пешка на клетке для превращения
                createChoiceMenu(board, to_choose, rectangles_to_choose, textures, OFFSETX, OFFSETY, CELLSIZE); // создаем и отрисовываем меню выбора
                drawChoiceMenu(window, to_choose, rectangles_to_choose);
            }
            nextModeBtnShape.setFillColor(sf::Color::Blue);
            drawNextModeButton(window,nextModeBtnShape,font,CELLSIZE,OFFSETX,OFFSETY); // отрисовываем кнопку перехода к след режиму
        }
        window.display(); // показывалось окно чтобы
    }
    delete board; // отчищаем память от твоей доски 😥😥😣😣😥
}
    


void solvePuzzle(sf::RenderWindow& window,sf::Font& font, std::string solvingPuzzleNum){
    const float CELLSIZE = 100.f; // размер клетки
    const float OFFSETX = 50.f; // отстпуп для букв слева
    const float OFFSETY = 50.f; // отступ для цифр снизу
    
    //буквы
    sf::Text letters[8];
    createLetters(letters, font, CELLSIZE, OFFSETX, OFFSETY);

    //цифры
    sf::Text numbers[8];
    createNumbers(numbers, font, CELLSIZE, OFFSETY);

    std::map<std::string, sf::Texture> textures; // мапа текстур
    loadTextures(textures);
    Board* board = new Board();  // создание твоей доски (❁´◡`❁)

    board->startRep(makePuzzleFilename(solvingPuzzleNum),textures);

    bool isFigureSelected = false; // фигура сейчас выбрана для хода
    figure* selectedFigure = nullptr; 
    std::vector<std::pair<int, int>> possibleMoves; // возможные ходы для выбранной фигуры

    bool needToCheckPositions = false; // нужно сравнить с сохранением после превращения

    bool endGameScreen=false; // должен ли быть экран завершения игры?
    bool win;
    sf::RectangleShape newGameButtonRect; // прямоугольник для начала новой игры

    std::vector<sf::Sprite> to_choose; // спрайты фигур в меню выбора
    std::vector<sf::RectangleShape> rectangles_to_choose; // прямоугольники на заднем плане в меню выбора

    sf::RectangleShape lastMoveFrom(sf::Vector2f(CELLSIZE, CELLSIZE)); // квадратик откуда последний ход
    sf::RectangleShape lastMoveTo(sf::Vector2f(CELLSIZE, CELLSIZE)); // квадратик куда последний ход
    lastMoveFrom.setFillColor(sf::Color(0, 255, 0, 80)); // цвет
    lastMoveTo.setFillColor(sf::Color(0, 255, 0, 80)); // цвет
    bool hasMoved = false; // флаг был ли уже ход а то при запуске когда хода не было сделано эти квадраты просто на угол уезжали и закрывали часть окна

    // создаем доску
    sf::RectangleShape boardRectangles[8][8];
    initializeBoardRectangles(boardRectangles, CELLSIZE, OFFSETX, OFFSETY);

    while (window.isOpen()){//             мод человек сколько и команда
        sf::Event event;  // какое событие происходит сейчас клик мыши или закрытие окно
        while (window.pollEvent(event)) { // получаем постоянно событие какое то
            handleWindowClose(window, event); // чтобы закрывалось окно
            if (event.type == sf::Event::MouseButtonPressed && // нажатие левой кнопки мыши
                event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y); // получаем положение курсора
                if (endGameScreen){ // если игра закончилась проверяем нажата ли кнопка начать новую игру и начинаем ее
                    if (newGameButtonRect.getGlobalBounds().contains(mousePos)){
                        createMainMenu(window,font);
                    }
                    continue;
                }
                // если пешка на клетке для превращения
                if (board->convertFlag){
                    if (!selectFigureToConvert(board, rectangles_to_choose, mousePos, textures, OFFSETX, CELLSIZE)) // если не превратилась еще то пропускаем иначе сравнием позиции
                        continue;
                    else
                        needToCheckPositions = true;
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
                        OFFSETY, CELLSIZE, false
                    );

                    if (moved) { // если походили тоесть applyMoveifValid вернуло true
                        int result = board->processWhiteMove(); // проверяем ход
                        if (result == 0) { // был не туда ход
                            win = false; 
                            endGameScreen = true;
                        }
                        else if (result == 2) { // ходы закончились
                            win = true;
                            endGameScreen = true;
                        }
                        hasMoved = !board->convertFlag;
                        lastMoveFrom.setPosition((board->getLastBlackFrom()).first*CELLSIZE+OFFSETX,(7-(board->getLastBlackFrom()).second)*CELLSIZE+OFFSETY);
                        lastMoveTo.setPosition((board->getLastBlackTo()).first*CELLSIZE+OFFSETX,(7-(board->getLastBlackTo()).second)*CELLSIZE+OFFSETY);
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

        window.clear(sf::Color(128,128,128)); // отчищаем окно чтобы оно обновлялось цвет в скобках это цвет фона (серый)

        drawBoardAndLabels(window, boardRectangles, letters, numbers); // рисуем доску и цифры буквы
        if (hasMoved) { // если ход был рисуем зеленые квадраты на последнем ходу
            window.draw(lastMoveFrom);
            window.draw(lastMoveTo);
        }
        if (needToCheckPositions){
            int result = board->processWhiteMove();
             if (result == 0) { // был не туда ход
                win = false; 
                endGameScreen = true;
            }
            else if (result == 2) { // ходы закончились
                win = true;
                endGameScreen = true;
            }
            needToCheckPositions = false;
        }
        if (board->isKingInCheck(figure::BLACK)){ // если какому нибудь королю стоит шах нарисовать красный квадрат на нем
            drawCheck(window,board,figure::BLACK,OFFSETX,OFFSETY,CELLSIZE);
        }else if(board->isKingInCheck(figure::WHITE)){
            drawCheck(window,board,figure::WHITE,OFFSETX,OFFSETY,CELLSIZE);
        }

        drawFigures(window, board, CELLSIZE, OFFSETX, OFFSETY); // рисуем фигуры

        if (isFigureSelected) { // если фигура выбрана
            drawMoveHighlights(window, possibleMoves, *board, selectedFigure, OFFSETX, OFFSETY, CELLSIZE); // рисуем возможные ходы
        }

        if (board->convertFlag){ // если пешка на клетке для превращения
            createChoiceMenu(board, to_choose, rectangles_to_choose, textures, OFFSETX, OFFSETY, CELLSIZE); // создаем и отрисовываем меню выбора
            drawChoiceMenu(window, to_choose, rectangles_to_choose);
        }

        if (endGameScreen)
            drawEndGameScreenPuzzle(window,win,font,newGameButtonRect);
        
        window.display(); // показывалось окно чтобы
    }
    delete board; // отчищаем память от твоей доски 😥😥😣😣😥
}