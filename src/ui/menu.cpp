#include "menu.hpp"


sf::RectangleShape makeButton(float w, float h, sf::Color fill, sf::Color outline) { // функция создания кнопки
    sf::RectangleShape btn({w, h}); // создаем кнопки с заданой шириной и высотой
    btn.setFillColor(fill); // цвет
    btn.setOutlineColor(outline); // цвет рамки
    btn.setOutlineThickness(2.f); // ширина рамки
    btn.setOrigin(w/2, h/2); // ставим начало коорд у кнопки в ее середину чтобы удобно поставить ее в центр экрана например
    return btn;
}


void drawLabel(sf::RenderWindow& win,const sf::Font& font,
               const sf::RectangleShape& btn, const std::string& s, unsigned size) // написать текст по центру кнопки
{
    sf::Text t(s, font, size); // текст
    t.setFillColor(sf::Color::Black); // цвет текста
    auto bb = t.getLocalBounds(); // квадрат в котором текст и ставим начальные коорды кнопки в его центр но чуть чуть выше
    t.setOrigin(bb.width/2, bb.height/2 + 5); // +5 потому что текст кажется не по центру а ниже
    t.setPosition(btn.getPosition()); // ставим коорды текста в кнопку
    win.draw(t); // рисуем текст
}

static void drawBackButton(sf::RenderWindow& win, sf::RectangleShape& backBtn, const sf::Font& font) { // создание кнопки back почти для каждого раздела меню
    const float W = 100, H = 40; 
    backBtn = makeButton(W, H, sf::Color(220, 100, 100), sf::Color::Black); // создаем кнопку
    backBtn.setPosition(win.getSize().x - W/2 - 10, win.getSize().y - H/2 - 10); // ставим на позицию
    win.draw(backBtn); // отрисовываем
    drawLabel(win, font, backBtn, "Back", 18); // и отрисовываем текст на ней
}

void drawButtonList( // вспомогательная функция для отрисовки похожих меню по названию кнопок
    sf::RenderWindow& win,
    std::map<std::string, sf::RectangleShape>& btns,
    sf::RectangleShape& backBtn,
    const sf::Font& font,
    const std::vector<std::pair<std::string, std::string>>& items,
    float btnW, float btnH,
    sf::Color fillColor,
    sf::Color outlineColor,
    bool includeBack = true
) {
    btns.clear();
    auto center = sf::Vector2f(win.getSize()) * 0.5f;
    float margin = 20.f;
    float total = items.size() * btnH + (items.size() - 1) * margin;
    float startY = center.y - total / 2 + btnH / 2;
    for (size_t i = 0; i < items.size(); ++i) {
        auto& [id, label] = items[i];
        auto btn = makeButton(btnW, btnH, fillColor, outlineColor);
        btn.setPosition(center.x, startY + i * (btnH + margin));
        btns[id] = btn;
        win.draw(btn);
        drawLabel(win, font, btn, label);
    }
    if (includeBack) {
        drawBackButton(win, backBtn, font);
    }
}

void drawMainMenu(sf::RenderWindow& win, std::map<std::string, sf::RectangleShape>& btns, sf::Font& font) {
    std::vector<std::pair<std::string, std::string>> items;
    if (savesExists()) {
        items = { {"continue","Continue"}, {"play","Play"}, {"puzzles","Puzzles"}, {"exit","Exit"} };
    } else {
        items = { {"play","Play"}, {"puzzles","Puzzles"}, {"exit","Exit"} };
    }
    drawButtonList(win, btns, *(sf::RectangleShape*)nullptr, font,
                   items, 300.f, 60.f, sf::Color(100,149,237), sf::Color::Black, false);
}

void drawGameTypeMenu(sf::RenderWindow& win, std::map<std::string, sf::RectangleShape>& btns,
                      sf::RectangleShape& backBtn, sf::Font& font) {
    std::vector<std::pair<std::string,std::string>> items = {
        {"classic","Classic"}, {"fischer","Fischer"}, {"three","3-Check"}
    };
    drawButtonList(win, btns, backBtn, font,
                   items, 280.f, 60.f, sf::Color(255,215,0), sf::Color::Black);
}

void drawOpponentMenu(sf::RenderWindow& win, std::map<std::string, sf::RectangleShape>& btns,
                      sf::RectangleShape& backBtn, sf::Font& font) {
    std::vector<std::pair<std::string,std::string>> items = {
        {"pvp","Vs Player"}, {"pve","Vs Computer"}
    };
    drawButtonList(win, btns, backBtn, font,
                   items, 220.f, 50.f, sf::Color(173,216,230), sf::Color::Black);
}

void drawColorMenu(sf::RenderWindow& win, std::map<std::string, sf::RectangleShape>& btns,
                   sf::RectangleShape& backBtn, sf::Font& font) {
    std::vector<std::pair<std::string,std::string>> items = {
        {"white","White"}, {"black","Black"}
    };
    drawButtonList(win, btns, backBtn, font,
                   items, 180.f, 50.f, sf::Color(240,240,240), sf::Color::Black);
}

void drawDifficultMenu(sf::RenderWindow& win, std::map<std::string, sf::RectangleShape>& btns,
                       sf::RectangleShape& backBtn, sf::Font& font) {
    std::vector<std::pair<std::string,std::string>> items = {
        {"easy","Easy"}, {"normal","Normal"}, {"hard","Hard"}, {"insane","Insane"}
    };
    drawButtonList(win, btns, backBtn, font,
                   items, 180.f, 50.f, sf::Color(50,50,130), sf::Color::Black);
}


void drawSaveMenu(sf::RenderWindow& win,
                  std::map<std::string, sf::RectangleShape>& btns,
                  sf::RectangleShape& backBtn,
                  const sf::Font& font,
                  const std::vector<std::string>& saves,
                  float scrollOffset)
{
    btns.clear();
    const float W = win.getSize().x * 0.7f;
    const float H = 40.f;
    const float M = 10.f;                  
    sf::Vector2f center{win.getSize()};
    center *= 0.5f;

    float startY = 10.f + H/2 - scrollOffset; // начало кнопок по y

    float centerX = win.getSize().x * 0.5f; // горизонтальный центр

    for (size_t i = 0; i < saves.size(); ++i) {
        float y = startY + i * (H + M);
        if (y + H/2 < 0 || y - H/2 > win.getSize().y)  // если кнопка целиком ушла за верх или низ — пропускаем
            continue;

        const std::string& name = saves[i];
        auto btn = makeButton(W, H, sf::Color(200,200,200), sf::Color::Black); // рисуем кнопочки
        btn.setPosition(centerX, y);
        btns[name] = btn;
        win.draw(btn);
        drawLabel(win, font, btn, name, 18);
    }

    // кнопка back
    drawBackButton(win, backBtn, font);
}

void drawPuzzleMenu(sf::RenderWindow& win,
                    std::map<std::string, sf::RectangleShape>& numberBtns,
                    sf::RectangleShape& backBtn,
                    sf::RectangleShape& createBtn,
                    sf::RectangleShape& deleteModeBtn,
                    sf::Font& font,
                    std::vector<int>& puzzleNumbers)
{
    numberBtns.clear();
    int totalRows = 0;
    const float BTN = 50, SP = 10; // размер кнопок и расстояние между ними
    const int cols = 14; // количесво колонок с кнопками 14 - максимум что влезает в экран с текущими параметрами
    sf::FloatRect area(30, 20, win.getSize().x-20, win.getSize().y-25); // создаем область для кнопок
    float y0 = area.top;
    for (int i = 0; i < puzzleNumbers.size(); ++i) {// размещаем кнопки
        int puzzleNum = puzzleNumbers[i];
        int r = i / cols, c = i % cols; // строка и колонка для кнопки
        totalRows = std::max(totalRows, r + 1);
        auto btn = makeButton(BTN, BTN, sf::Color(200,200,200), sf::Color::Black); // создаем кнопку
        btn.setPosition(area.left + c*(BTN+SP) + BTN/2,
                        y0 + r*(BTN+SP) + BTN/2);
        numberBtns[std::to_string(puzzleNum)] = btn; // записываем эту кнопку в массив
        win.draw(btn);
        drawLabel(win, font, btn, std::to_string(puzzleNum), 20); // рисуем надпись на кнопке
    }

    const float hintW = 300, hintH = 35; // надпись rmb to edit
    auto hintBtn = makeButton(hintW, hintH, sf::Color(230, 230, 230), sf::Color::Black);
    float hintX = area.left + hintW / 2;
    float hintY = y0 + totalRows * (BTN + SP) + hintH; // под кнопками
    hintBtn.setPosition(hintX, hintY);
    win.draw(hintBtn);
    drawLabel(win, font, hintBtn, "RMB to edit puzzle", 18);

    const float CW = 700, CH = 40; // параметры кнопки создать ширина и высота
    createBtn = makeButton(CW, CH, sf::Color(100,220,100), sf::Color::Black); // кнопка
    createBtn.setPosition(area.left + CW/2, win.getSize().y - CH/2 - 10);
    win.draw(createBtn);

    win.draw(deleteModeBtn); // кнопка для включения режима удаления
    drawLabel(win, font, deleteModeBtn, "Delete Mode", 18);
    drawLabel(win, font, createBtn, "Create", 18); // текст
    drawBackButton(win, backBtn, font); // создаем кнопку back
}

void createMainMenu(sf::RenderWindow& window, sf::Font& font){ // перенесена в game.cpp чтобы после окончания игры можно было меню открыть
    enum MainMenuModes {MainMenu, Puzzles, ModeChoose ,EnemyChoose, TeamChoose, DifficultChoose, Continue, DEBUG}; // экраны в меню ЧТОБЫ ОТКЛЮЧИТЬ МЕНЮ DEBUG


    MainMenuModes currentMode = MainMenu; // заменить MainMenu на DEBUG для отключения меню будет выбран режим по параметрам ниже
    int mode = 1; //тут по идее мы делаем от одного до 3, где 1 - дефлот шахматы, 2 - фишер, 3 - до 3 шахов
    int player = 1; //если 1 - игра с компом, если 2 - два игрока
    int difficult = 5; // сложность 1-20
    figure::teams userTeam = figure::WHITE; 
    bool needToQuitMenuFlag = false;
    bool createFlag = false; // флаг для входа в режим создания
    std::string solvingPuzzleNum = "-1"; // номер решаемого пазла -1 - не решается пазл сейчас
    bool deleteMode = false; // режим удаления пазлов
    std::vector<int> puzzleNumbers = getPuzzleNumbers();
    std::string editingPuzzleNum = "-1";

    auto saves = getSaveFilesWithoutDotFen(); 
    float scrollOffset = 0.f; // где начало скрола
    const float SAVEMENU_BTN_H = 40.f; // высота кнопок в savemenu
    const float SAVEMENU_GAP   = 10.f; // расстояние между кнопками в savemenu


    std::map<std::string,sf::RectangleShape> menuButtonsRects; // мапа кнопок и их id на экране
    sf::RectangleShape backBtn; // кнопка назад 
    sf::RectangleShape createPuzzleBtn; // кнопка создания задачи
    sf::RectangleShape deletePuzzleModeBtn; // кнопка для включения режима удаления задач
    const float CW = 700, CH = 40;
    sf::FloatRect area(30, 20, window.getSize().x-20, window.getSize().y-25); // создаем область для кнопок
    deletePuzzleModeBtn = makeButton(CW, CH, sf::Color::Red, sf::Color::Black);
    deletePuzzleModeBtn.setOrigin(CW/2, CH/2);
    deletePuzzleModeBtn.setPosition(area.left + CW/2, window.getSize().y - CH*1.5f - 20);

    std::string clickedButtonID; // и id кнопки на которую нажали
    while (window.isOpen()){ // основной цикл для меню заканчивается когда меню закрывается и открывается игра
        window.clear(sf::Color(128,128,128));
        if (currentMode == DEBUG) // если DEBUG то сразу закрываем меню
            break;
        if (currentMode == MainMenu){ // отрисовывем нужное меню
            drawMainMenu(window,menuButtonsRects,font);
        }
        else if (currentMode == Continue){
            drawSaveMenu(window,menuButtonsRects,backBtn,font,saves,scrollOffset);
        }
        else if (currentMode == ModeChoose){
            drawGameTypeMenu(window,menuButtonsRects,backBtn,font);
        }
        else if (currentMode == Puzzles){
            drawPuzzleMenu(window,menuButtonsRects,backBtn,createPuzzleBtn,deletePuzzleModeBtn,font,puzzleNumbers);
        }
        else if (currentMode == EnemyChoose){
            drawOpponentMenu(window,menuButtonsRects,backBtn,font);
        }
        else if (currentMode == TeamChoose){
            drawColorMenu(window,menuButtonsRects,backBtn,font);
        }
        else if (currentMode == DifficultChoose){
            drawDifficultMenu(window,menuButtonsRects,backBtn,font);
        }
        window.display();
        sf::Event event;  // какое событие происходит сейчас клик мыши или закрытие окно
        while (window.pollEvent(event)){ // получаем постоянно событие какое то
            handleWindowClose(window, event); // чтобы закрывалось окно TODO чтобы делало сейвы
            clickedButtonID = ""; // обнуляем кнопку а то back может несколько раз нажаться при однократном нажатии
            if (currentMode == Continue){ // кнопка продолжить
                std::string clickedSave = handleSaveMenuEvents(window, event, menuButtonsRects, scrollOffset, saves.size(), SAVEMENU_BTN_H, SAVEMENU_GAP);
                if (!clickedSave.empty()) { // если нажата кнопка сохранения
                    size_t underscorePos = clickedSave.find('_'); // из названия файла получаем режим и противника
                    size_t secondUnderscorePos = clickedSave.find('_', underscorePos + 1);
                    size_t dotPos = clickedSave.find('.', underscorePos);
                    std::string mode = clickedSave.substr(underscorePos + 1, secondUnderscorePos - underscorePos - 1);
                    std::string opponent = clickedSave.substr(secondUnderscorePos + 1, dotPos - secondUnderscorePos - 1);
                    clickedSave+=".fen";
                    if (opponent == "vsComputer" and mode == "Classic") // запускаем функцию подходящию под выбранные настройки
                        vsComputerStandart(window, font,5, userTeam, clickedSave);
                    else if (opponent == "vsComputer" and mode == "Fisher")
                        vsComputerFisher(window, font,5, userTeam, clickedSave);
                    else if (opponent == "vsComputer" and mode == "3Check")
                        vsComputer3Check(window, font,5, userTeam, clickedSave);
                    else if (opponent == "vsPlayer" and mode == "Classic")
                        vsPlayerStandart(window, font, clickedSave);
                    else if (opponent == "vsPlayer" and mode == "Fisher")
                        vsPlayerFisher(window, font, clickedSave);
                    else if (opponent == "vsPlayer" and mode == "3Check")
                        vsPlayer3Check(window, font, clickedSave);
                }
            }
            if (event.type == sf::Event::MouseButtonPressed && // нажатие левой кнопки мыши
                event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y); // получаем положение курсора
                if (backBtn.getGlobalBounds().contains(mousePos)){ 
                    clickedButtonID = "back";
                }
                else if (currentMode == Puzzles and createPuzzleBtn.getGlobalBounds().contains(mousePos)){
                    clickedButtonID = "create";
                }
                else if (currentMode == Puzzles and deletePuzzleModeBtn.getGlobalBounds().contains(mousePos)){
                    clickedButtonID = "deletemode";
                }
                else{
                    for (auto& [id, rect] : menuButtonsRects) { // по всему списку кнопок проходимся
                        if (rect.getGlobalBounds().contains(mousePos)){
                            clickedButtonID = id;
                        }
                    } 
                }
            }
            else if (event.type == sf::Event::MouseButtonPressed && // нажатие правой кнопки мыши
                event.mouseButton.button == sf::Mouse::Right){
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y); // получаем положение курсора
                for (auto& [id, rect] : menuButtonsRects) { // по всему списку кнопок проходимся
                        if (rect.getGlobalBounds().contains(mousePos)){
                            clickedButtonID = "edit_" + id;
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
            else if (clickedButtonID == "continue"){
                currentMode = Continue;
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
                currentMode = DifficultChoose;
            }
            else if (clickedButtonID == "white"){
                userTeam = figure::WHITE;
                currentMode = DifficultChoose;
            }
            else if (clickedButtonID == "easy"){
                difficult = 5;
                needToQuitMenuFlag = true;
                break;
            }
            else if (clickedButtonID == "normal"){
                difficult = 10;
                needToQuitMenuFlag = true;
                break;
            }
            else if (clickedButtonID == "hard"){
                difficult = 15;
                needToQuitMenuFlag = true;
                break;
            }
            else if (clickedButtonID == "insane"){
                difficult = 20;
                needToQuitMenuFlag = true;
                break;
            }
            else if (clickedButtonID == "create"){
                createFlag = true;
                needToQuitMenuFlag = true;
                break;
            }
            else if (clickedButtonID == "deletemode"){
                deleteMode = !deleteMode;
                deletePuzzleModeBtn.setFillColor(deleteMode ? sf::Color(0,255,0):sf::Color(255,0,0));
            }

            else if (!clickedButtonID.empty() && std::all_of(clickedButtonID.begin(), clickedButtonID.end(), ::isdigit)){
                if (deleteMode){ // если включен режим удаления
                    deletePuzzleByNumber(clickedButtonID); // удалить пазл
                    puzzleNumbers = getPuzzleNumbers();
                }
                else{
                    solvingPuzzleNum=clickedButtonID; // иначе запустить пазл
                    needToQuitMenuFlag = true;
                    break;
                }
            }
            else if (!clickedButtonID.empty() &&
                    clickedButtonID.substr(0, 5) == "edit_" && // изменение пазла
                    std::all_of(clickedButtonID.begin() + 5, clickedButtonID.end(), ::isdigit)) {
                std::string numStr = clickedButtonID.substr(5);
                editingPuzzleNum = numStr;
                needToQuitMenuFlag = true;
                break;
            }

            else if (clickedButtonID == "back"){
                if (currentMode == ModeChoose)
                    currentMode = MainMenu;
                else if (currentMode == Continue)
                    currentMode = MainMenu;
                else if (currentMode == Puzzles)
                    currentMode = MainMenu;
                else if (currentMode == EnemyChoose)
                    currentMode = ModeChoose;
                else if (currentMode == TeamChoose)
                    currentMode = EnemyChoose;
                else if (currentMode == TeamChoose)
                    currentMode = EnemyChoose;
                else if (currentMode = DifficultChoose)
                    currentMode = TeamChoose;
            }
        }
        if (needToQuitMenuFlag)
            break;
    }
    if (createFlag)  // запускаем функцию подходящию под выбранные настройки
        createPuzzle(window,font,editingPuzzleNum);// если editingPuzzleNum != -1 то это изменение пазла иначе создание
    else if (solvingPuzzleNum != "-1")
        solvePuzzle(window,font,solvingPuzzleNum);       
    else if (player == 1 and mode == 1)
        vsComputerStandart(window,font, difficult,userTeam);
    else if (player == 1 and mode == 2)
        vsComputerFisher(window,font, difficult, userTeam);
    else if (player == 1 and mode == 3)
        vsComputer3Check(window,font, difficult, userTeam);
    else if (player == 2 and mode == 1)
        vsPlayerStandart(window,font);
    else if (player == 2 and mode == 2)
        vsPlayerFisher(window,font);
    else if (player == 2 and mode == 3)
        vsPlayer3Check(window,font);
}