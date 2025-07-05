#include "../../include/king.hpp"

king::king(teams t, std::pair<int,int> p) {
        team = t;
        pos = p;
        figureType = KING;

        if (team == WHITE)
            iconPath = "/materials/nw.png";
        else
            iconPath = "/materials/nb.png";
}

std::vector<std::pair<int, int>> king::get_available_moves(const Board& board) { //TODO сделать чтобы противники учитывались там передавать доску и проверять
    std::vector<std::pair<int, int>> moves;

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;

            int nx = pos.first + dx;
            int ny = pos.second + dy;

            if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
                moves.push_back({nx, ny});
            }
        }
    }

    return moves;
}//TODO ракировка
