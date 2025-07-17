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

void vsComputerStandart(sf::RenderWindow& window,sf::Font& font, figure::teams userTeam, std::string savefile){// отдельная функция для игры против компьютера
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
    engine.sendCommand("isready"); //проверяем готовность
    //engine.sendCommand("position fen 4k3/8/8/8/8/8/PPPPPPPP/4K3 w - - 0 1");
    while (window.isOpen()) { // основной цикл постоянно повторяется пока окно открыто
        if (board->getCurrentTeam() != userTeam) { //проверяем когда ходит бот
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

void vsComputerFisher(sf::RenderWindow& window,sf::Font& font, figure::teams userTeam, std::string savefile){// отдельная функция для игры против компьютера
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
    engine.sendCommand("isready"); //проверяем готовность
    engine.sendCommand(std::string("position fen ") + board->fenPos); //сообщаем позицию боту


    while (window.isOpen()) { // основной цикл постоянно повторяется пока окно открыто
        
        processEvents(window, font, board, 2, 1, endGameScreen, newGameButtonRect, isFigureSelected, selectedFigure, possibleMoves, lastMoveFrom, lastMoveTo, textures, to_choose, rectangles_to_choose, hasMoved, OFFSETX, OFFSETY, CELLSIZE, &rightCastle, &leftCastle); // обрабатываем все возможные события клик мыши и тд

        if (board->getCurrentTeam() != userTeam) {
        // 1.2) Формируем строку всех предыдущих ходов
        std::string moves;
        for (const auto& m : board->movesUCI) {
            moves += m + " ";
        }
        // 1.3) Отправляем движку позицию и ждём лучший ход
        engine.sendCommand("position fen " + board->fenPos + " moves " + moves);
        engine.sendCommand("go movetime 1000");

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

        // 1.4) Разбираем UCI-строку bestmove
        int fx = bestmove[0] - 'a';
        int fy = bestmove[1] - '1';
        int tx = bestmove[2] - 'a';
        int ty = bestmove[3] - '1';
        char prom = bestmove.size() >= 5 ? bestmove[4] : '\0';

        // 1.5) Делаем ход через ваш метод
        board->makeMove({fx, fy}, {tx, ty});
         lastMoveFrom.setPosition(OFFSETX + fx * CELLSIZE,
                                 OFFSETY + (7 - fy) * CELLSIZE);
        lastMoveTo  .setPosition(OFFSETX + tx * CELLSIZE,
                                 OFFSETY + (7 - ty) * CELLSIZE);
        hasMoved = true;

        // 1.6) Если была превращение — вызываем convertPawn
        if (prom) {
            figure::figureTypes newType;
            switch (prom) {
                case 'r': newType = figure::ROOK;   break;
                case 'b': newType = figure::BISHOP; break;
                case 'n': newType = figure::KNIGHT; break;
                case 'q': default:  newType = figure::QUEEN;
            }
            // текстура выбирается по цвету ботa
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

        if (board->convertFlag){ // если пешка на клетке для превращения
            createChoiceMenu(board, to_choose, rectangles_to_choose, textures, OFFSETX, OFFSETY, CELLSIZE); // создаем и отрисовываем меню выбора
            drawChoiceMenu(window, to_choose, rectangles_to_choose);
        }

        window.display(); // показывалось окно чтобы
    }
    delete board; // отчищаем память от твоей доски 😥😥😣😣😥
}

void vsComputer3Check(sf::RenderWindow& window,sf::Font& font, figure::teams userTeam, std::string savefile){// отдельная функция для игры против компьютера
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
    engine.sendCommand("isready"); //проверяем готовность
    //engine.sendCommand("position fen 4k3/8/8/8/8/8/PPPPPPPP/4K3 w - - 0 1");
    while (window.isOpen()) { // основной цикл постоянно повторяется пока окно открыто
        if (board->getCurrentTeam() != userTeam) { //проверяем когда ходит бот
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
// TODO разбить этот код на функции а то чет слишком много повторов


void createPuzzle(sf::RenderWindow& window,sf::Font& font){
    const float CELLSIZE = 100.f; // размер клетки
    const float OFFSETX = 50.f; // отстпуп для букв слева
    const float OFFSETY = 50.f; // отступ для цифр снизу
    
    enum createPuzzleModes {Setup,Game};

    createPuzzleModes currentMode = Setup;

    //буквы
    sf::Text letters[8];
    createLetters(letters, font, CELLSIZE, OFFSETX, OFFSETY);

    //цифры
    sf::Text numbers[8];
    createNumbers(numbers, font, CELLSIZE, OFFSETY);

    std::map<std::string, sf::Texture> textures; // мапа текстур
    loadTextures(textures);
    Board* board = new Board();  // создание твоей доски (❁´◡`❁)
    board->clear();

    std::pair<int,int> selectedCell = std::make_pair(-1,-1);
    bool cellIsSelected = false; 
    int choosedFigurePos;

    std::vector<sf::Sprite> figureSpritesToChoose; // спрайты фигур в меню выбора
    std::vector<sf::RectangleShape> rectanglesToChoose; // прямоугольники на заднем плане в меню выбора
    sf::RectangleShape container; 

    sf::RectangleShape nextModeBtnShape;

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
        if (currentMode == Setup){
            while (window.pollEvent(event)) {
                handleWindowClose(window,event);
                if (event.type == sf::Event::MouseButtonPressed && // нажатие левой кнопки мыши
                event.mouseButton.button == sf::Mouse::Left){
                    sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y); // получаем положение курсора
                    if (nextModeBtnShape.getGlobalBounds().contains(mousePos)){
                        currentMode = Game;
                        continue;
                    }
                    if (cellIsSelected){
                        choosedFigurePos = handleCreatePuzzleEvents(mousePos,container,CELLSIZE);
                        if (choosedFigurePos != -1) {
                            // Позиции: 0–5 — белые фигуры, 6–11 — черные
                            std::vector<std::string> types = {"k", "q", "b", "n", "r", "p"};
                            int typeIndex = choosedFigurePos % 6;
                            bool isWhite = choosedFigurePos < 6;
                            std::string figure = types[typeIndex] + (isWhite ? "w" : "b");

                            char typeChar = figure[0];  // 'k', 'q', 'b', 'n', 'r', 'p'
                            char colorChar = figure[1]; // 'w' или 'b'

                            figure::teams team = (colorChar == 'w') ? figure::teams::WHITE : figure::teams::BLACK;

                            if (typeChar == 'k')
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

                            selectedCell = std::make_pair(-1,-1);
                            cellIsSelected = false;
                            continue;
                        }
                    }
                    
                    for (int x = 0; x<8; x++){
                        for (int y = 0; y<8; y++){
                            if (boardRectangles[x][y].getGlobalBounds().contains(mousePos)){
                                selectedCell=std::make_pair(x,7-y);
                                cellIsSelected = true;
                                choosedCellRect.setPosition(
                                OFFSETX + selectedCell.first * CELLSIZE,
                                OFFSETY + (7 - selectedCell.second) * CELLSIZE);
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
        else if(currentMode == Game){
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
                            //TODO сохраняем новую позицию
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
            
            drawNextModeButton(window,nextModeBtnShape,font,CELLSIZE,OFFSETX,OFFSETY);
        }
        window.display(); // показывалось окно чтобы
    }
    delete board; // отчищаем память от твоей доски 😥😥😣😣😥
}
    