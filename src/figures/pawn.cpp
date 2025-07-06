#include "../../include/pawn.hpp"
#include "board.hpp"

pawn::pawn(teams t, std::pair<int,int> p) {
        team = t;
        pos = p;
        figureType = PAWN;

        if (team == WHITE)
            iconPath = "/materials/pw.png";
        else
            iconPath = "/materials/pb.png";
}

std::vector<std::pair<int, int>> pawn::get_available_moves(const Board& board) {
    std::vector<std::pair<int, int>> moves;

    int direction = (team == WHITE) ? +1 : -1;

    int nx = pos.first;
    int ny = pos.second + direction;

        if (ny >= 0 && ny < 8) { //чисто ходить прямо
        if (!board.isOccupied(nx, ny)) {
            moves.push_back({nx, ny});
        }
    }

    if ((team == WHITE && pos.second == 1) || (team == BLACK && pos.second == 6)) { //если на начальной клетке стоит, то может ходить на 2 клетки
        int ny_one_step = pos.second + direction; //на одну клетку
        int ny_two_steps = pos.second + 2 * direction; //на две

        if (!board.isOccupied(nx, ny_one_step) && !board.isOccupied(nx, ny_two_steps) && ny_two_steps >= 0 && ny_two_steps < 8) {
            moves.push_back({nx, ny_two_steps}); //проверяем все условия и если можем, идем шагаем на две клетки
        }
    }

    return moves;
}

//TODO сделать взятие по диагонали и превращение пешки в другую фигуру