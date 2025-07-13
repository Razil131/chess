#include <SFML/Graphics.hpp>
#include "game.hpp"
#include "board.hpp"
#include "engine.hpp"
#include <iostream>

void vsPlayer(sf::RenderWindow& window,sf::Font& font){    // отдельная функция для игры против игрока
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

    // создаем доску
    sf::RectangleShape boardRectangles[8][8];
    initializeBoardRectangles(boardRectangles, CELLSIZE, OFFSETX, OFFSETY);

    while (window.isOpen()){
        processEvents(window, board, endGameScreen, newGameButtonRect, isFigureSelected, selectedFigure, possibleMoves, lastMoveFrom, lastMoveTo, textures, to_choose, rectangles_to_choose, hasMoved, OFFSETX, OFFSETY, CELLSIZE); // обрабатываем все возможные события клик мыши и тд

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

void vsComputer(sf::RenderWindow& window,sf::Font& font, figure::teams userTeam){// отдельная функция для игры против компьютера
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
        processEvents(window, board, endGameScreen, newGameButtonRect, isFigureSelected, selectedFigure, possibleMoves, lastMoveFrom, lastMoveTo, textures, to_choose, rectangles_to_choose, hasMoved, OFFSETX, OFFSETY, CELLSIZE); // обрабатываем все возможные события клик мыши и тд

        

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

int main() {
    sf::RenderWindow window(sf::VideoMode(900, 900), "Chess", sf::Style::Titlebar | sf::Style::Close); // создаем окошечно так сказать 900 на 900 пикселей с название Chess

    sf::Font font; // шрифт
    if (!loadFont(font)) {
        return -1; // если не загрузился то :(
    }

    enum MainMenuModes {MainMenu, Puzzles, ModeChoose ,EnemyChoose, TeamChoose, DEBUG}; // экраны в меню ЧТОБЫ ОТКЛЮЧИТЬ МЕНЮ DEBUG
    
    MainMenuModes currentMode = MainMenu; // заменить MainMenu на DEBUG для отключения меню будет выбран режим по параметрам ниже
    int mode = 1; //тут по идее мы делаем от одного до 3, где 1 - дефлот шахматы, 2 - фишер, 3 - до 3 шахов
    int player = 1; //если 1 - игра с компом, если 2 - два игрока
    int dif = 1; //TODO потом switch case сделать чтобы выбирать сложность
    figure::teams userTeam = figure::WHITE; 
    bool needToQuitMenuFlag = false;

    std::map<std::string,sf::RectangleShape> menuButtonsRects; // мапа кнопок и их id на экране
    sf::RectangleShape backBtn; // кнопка назад 
    sf::RectangleShape createPuzzleBtn; // кнопка создания задачи
    std::string clickedButtonID; // и id кнопки на которую нажали
    while (window.isOpen()){ // основной цикл для меню заканчивается когда меню закрывается и открывается игра
        if (currentMode == DEBUG) // если DEBUG то сразу закрываем меню
            break;
        window.clear(sf::Color(128,128,128));
        if (currentMode == MainMenu){ // отрисовывем нужное меню
            drawMainMenu(window,menuButtonsRects,font);
        }
        else if (currentMode == ModeChoose){
            drawGameTypeMenu(window,menuButtonsRects,backBtn,font);
        }
        else if (currentMode == Puzzles){
            drawPuzzleMenu(window,menuButtonsRects,backBtn,createPuzzleBtn,font,150);
        }
        else if (currentMode == EnemyChoose){
            drawOpponentMenu(window,menuButtonsRects,backBtn,font);
        }
        else if (currentMode == TeamChoose){
            drawColorMenu(window,menuButtonsRects,backBtn,font);
        }
        sf::Event event;  // какое событие происходит сейчас клик мыши или закрытие окно
        while (window.pollEvent(event)){ // получаем постоянно событие какое то
            clickedButtonID = ""; // обнуляем кнопку а то back может несколько раз нажаться при однократном нажатии
            handleWindowClose(window, event); // чтобы закрывалось окно
            if (event.type == sf::Event::MouseButtonPressed && // нажатие левой кнопки мыши
                event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y); // получаем положение курсора
                if (backBtn.getGlobalBounds().contains(mousePos)){ 
                    clickedButtonID = "back";
                }
                else{
                    for (auto& [id, rect] : menuButtonsRects) { // по всему списку кнопок проходимся
                        if (rect.getGlobalBounds().contains(mousePos)){
                            clickedButtonID = id;
                        }
                    } 
                }
            }
            if (clickedButtonID == "exit"){
                window.close();
            }
            else if (clickedButtonID == "play"){
                currentMode = ModeChoose;
            }
            else if (clickedButtonID == "puzzles"){
                currentMode = Puzzles;
            }
            else if (clickedButtonID == "classic"){
                mode = 1;
                currentMode = EnemyChoose;
            }
            else if (clickedButtonID == "fischer"){
                mode = 2;
                currentMode = EnemyChoose;
            }
            else if (clickedButtonID == "three"){
                mode = 3;
                currentMode = EnemyChoose;
            }
            else if (clickedButtonID == "pve"){
                player = 1;
                currentMode = TeamChoose;
            }
            else if (clickedButtonID == "pvp"){
                player = 2;
                needToQuitMenuFlag = true;
                break;
            }
            else if (clickedButtonID == "black"){
                userTeam = figure::BLACK;
                needToQuitMenuFlag = true;
                break;
            }
            else if (clickedButtonID == "white"){
                userTeam = figure::WHITE;
                needToQuitMenuFlag = true;
                break;
            }
            else if (clickedButtonID == "back"){
                if (currentMode == ModeChoose)
                    currentMode = MainMenu;
                else if (currentMode == Puzzles)
                    currentMode = MainMenu;
                else if (currentMode == EnemyChoose)
                    currentMode = ModeChoose;
                else if (currentMode == TeamChoose)
                    currentMode = EnemyChoose;
                else if (currentMode == TeamChoose)
                    currentMode = EnemyChoose;
            }
        }
        window.display();
        if (needToQuitMenuFlag)
            break;
    }
    if (player == 1 and mode == 1) // запускаем функцию подходящию под выбранные настройки
        vsComputer(window,font,userTeam);
    else if (player == 2 and mode == 1)
        vsPlayer(window,font);
    return 0;
}