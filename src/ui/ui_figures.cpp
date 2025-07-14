#include "ui_figures.hpp"

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
