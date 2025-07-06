#include "../../include/rook.hpp"
#include "board.hpp"

rook::rook(teams t, std::pair<int,int> p) {
        team = t;
        pos = p;
        figureType = ROOK;

        if (team == WHITE)
            iconPath = "/materials/rw.png";
        else
            iconPath = "/materials/rb.png";
}

std::vector<std::pair<int, int>> rook::get_available_moves(const Board& board) { 
    std::vector<std::pair<int, int>> moves;

    const std::vector<std::pair<int, int>> directions = { //направления движения ладьи
        {+1, 0}, {-1, 0}, {0, +1}, {0, -1}
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
