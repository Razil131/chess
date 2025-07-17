#include "events.hpp"

void processEvents(
    sf::RenderWindow& window,
    sf::Font& font,
    Board* board,
    int mode,
    int players,
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
        if (endGameScreen) // чтобы не сохранялось если игра завершена
            handleWindowClose(window, event); // чтобы закрывалось окно
        else
            handleWindowClose(window, event,board,players,mode);// чтобы закрывалось окно и сохранялась игра
        if (event.type == sf::Event::MouseButtonPressed && // нажатие левой кнопки мыши
            event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y); // получаем положение курсора
            if (endGameScreen){ // если игра закончилась проверяем нажата ли кнопка начать новую игру и начинаем ее
                if (newGameButtonRect.getGlobalBounds().contains(mousePos)){
                    createMainMenu(window,font);
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
                    {board->fisherCastle(false); // налево
                    }
                else if ((*rightCastle).getGlobalBounds().contains(mousePos)) // направо
                    {board->fisherCastle(true);
                    }
            }
        }
    }
}

// закрывает окно по крестику
void handleWindowClose(sf::RenderWindow& window, const sf::Event& event, Board* board, int players, int mode) {
    if (event.type == sf::Event::Closed) {
        if (board){
            board->exportToFile(getSaveFileName(mode,players)+".fen",players,mode);
        }
        window.close();
    }
}

// обработка колеса и нажатия в  SaveMenu
std::string handleSaveMenuEvents(sf::RenderWindow& win,
                                 sf::Event& event,
                                 std::map<std::string, sf::RectangleShape>& btns,
                                 float& scrollOffset,
                                 size_t savesCount,
                                 float buttonHeight,
                                 float gap,
                                 float scrollSpeed)
{

    if (event.type == sf::Event::MouseWheelScrolled && // если колесико крутят
        event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
    {
        scrollOffset -= event.mouseWheelScroll.delta * scrollSpeed; // скрол вниз или вверх меняем
        scrollOffset = scrollOffset > 0 ? scrollOffset : 0; // чтобы скрол был неотрицательным

        float totalHeight = savesCount * buttonHeight + (savesCount - 1) * gap; // получаем сколько вообще занимают кнопки по высоте
        float viewHeight  = static_cast<float>(win.getSize().y); // видимый Y у нас 900px
        float maxOffset   = std::max(0.f, totalHeight - viewHeight); // максимальная глубина прокрута

        scrollOffset = std::clamp(scrollOffset, static_cast<float>(0), maxOffset); // и кламп так называемый чтобы scrolloffset был между 0 и maxOffset
    } // по сохранению нажимают
    else if (event.type == sf::Event::MouseButtonPressed && // е
             event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f mp(event.mouseButton.x, event.mouseButton.y);
        for (auto& [key, btn] : btns) {
            if (btn.getGlobalBounds().contains(mp))
                return key;// возвращаем сохранение по которому нажали
        }
    }

    return "";
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