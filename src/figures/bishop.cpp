#include <cmath>
#include "../../include/bishop.hpp"
#include "board.hpp"

bishop::bishop(teams t, std::pair<int,int> p) {
        team = t; //конструктор
        pos = p;
        figureType = BISHOP;

        if (team == WHITE)
            iconPath = "/materials/bw.png";
        else
            iconPath = "/materials/bb.png";
}

std::vector<std::pair<int, int>> bishop::get_available_moves(const Board& board) {
    std::vector<std::pair<int, int>> moves;

    const std::vector<std::pair<int, int>> directions = { //тут направления, 4 диагонали, хз вроде так должно быть
        {+1, +1}, {+1, -1}, {-1, +1}, {-1, -1}
    };

    for (auto [dx, dy] : directions) {
        int nx = pos.first + dx; //первый ход по иксу
        int ny = pos.second + dy; //первый ход по игрику

        while (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
            if (board.isOccupied(nx, ny)) { // Проверяем занята ли клетка
                if (board.isOccupiedByOwnTeam(nx, ny, team)) break; //если своя фигура, то дальше не лезем
                if (board.isOccupiedByEnemyTeam(nx, ny, team)) { //если вражеская, то ее можем забрать и дальше не лезем
                    moves.emplace_back(nx, ny);
                    break;
                }
            } else {
                moves.emplace_back(nx, ny); //если нету ничего то тоже добавляем
            }
            nx += dx;
            ny += dy;
        }
    }
    return moves;
}