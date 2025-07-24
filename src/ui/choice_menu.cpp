#include "choice_menu.hpp"

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
        sprite.setScale(0.15f, 0.15f);

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
bool selectFigureToConvert(Board* board,
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
                    return true;
                case(1): // второй квадрат и так далее
                    board->convertPawn(board->convertPosition.first, board->convertPosition.second, figure::BISHOP, col==figure::WHITE ? textures["bw"] : textures["bb"]);
                    return true;
                case(2):
                    board->convertPawn(board->convertPosition.first, board->convertPosition.second, figure::KNIGHT, col==figure::WHITE ? textures["nw"] : textures["nb"]);
                    return true;
                case(3):
                    board->convertPawn(board->convertPosition.first, board->convertPosition.second, figure::ROOK, col==figure::WHITE ? textures["rw"] : textures["rb"]);
                    return true;
            }
        }
    }
    return false;
}