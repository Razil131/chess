#include "board.hpp"

figure* Board::getFigure(int x, int y) const{ //функция для возвращения указателя на фигуру
    if(x > 0 && x < 8 && y > 0 && y < 8){
        return board[x][y].get(); //если че то есть, вернет указатель
    }
    return nullptr; //если ничего нет
}

bool Board::isOccupied(int x, int y) const {
    return getFigure(x, y) != nullptr; //если клетка занята, вернется тру, если не занята - фолс
}

bool Board::isOccupiedByOwnTeam(int x, int y, figure::teams team) const{
    figure* fig = getFigure(x, y);
    return fig && fig->getTeam() == team; //если фигура есть и команда совпадает, возращаем тру
}

bool Board::isOccupiedByEnemyTeam(int x, int y, figure::teams team) const{
    figure* fig = getFigure(x, y);
    return fig && fig->getTeam() != team; //если фигура есть и команда не совпадает, возращаем тру
}

void Board::setFigure(int x, int y, std::unique_ptr<figure> fig) {
    if (x >= 0 && x < 8 && y >= 0 && y < 8)
        board[y][x] = std::move(fig); //устанавливает фигуру в нужное место
}

void Board::removeFigure(int x, int y) {
    if (x >= 0 && x < 8 && y >= 0 && y < 8)
        board[y][x].reset(); // Удаляет фигуру, устанавливает nullptr в ее место
}


