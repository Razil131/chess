#include "../../include/pawn.hpp"

pawn::pawn(teams t, std::pair<int,int> p) {
        team = t;
        pos = p;
        figureType = PAWN;

        if (team == WHITE)
            iconPath = "/materials/pw.png";
        else
            iconPath = "/materials/pb.png";
}

std::vector<std::pair<int, int>> pawn::get_available_moves() { //TODO сделать чтобы противники учитывались там передавать доску и проверять
    std::vector<std::pair<int, int>> moves;

    int direction = (team == WHITE) ? +1 : -1;

    int nx = pos.first;
    int ny = pos.second + direction;

    if (ny >= 0 && ny < 9) {
        moves.push_back({nx, ny});
    }

    // первый ход на 2 клетки мб как то по другому сделать
    if ((team == WHITE && pos.second == 1) ||
        (team == BLACK && pos.second == 6)) {
        ny = pos.second + 2 * direction;
        if (ny >= 0 && ny < 9) {
            moves.push_back({nx, ny});
        }
    }

    // TODO можно есть(кушать убивать пожирать жрать) по диагонали но ходить по диаганали низя
    for (int dx = -1; dx <= 1; dx += 2) {
        int nx_diag = pos.first + dx;
        int ny_diag = pos.second + direction;
        if (nx_diag >= 0 && nx_diag < 9 && ny_diag >= 0 && ny_diag < 9) {
            moves.push_back({nx_diag, ny_diag});
        }
    }

    return moves;
}
