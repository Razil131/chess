#include <cmath>
#include "../../include/bishop.hpp"

bishop::bishop(teams t, std::pair<int,int> p) {
        team = t; //конструктор
        pos = p;
        figureType = BISHOP;

        if (team == WHITE)
            iconPath = "/materials/bw.png";
        else
            iconPath = "/materials/bb.png";
}

std::vector<std::pair<int, int>> bishop::get_available_moves(const Board& board) { //TODO сделать чтобы противники учитывались там передавать доску и проверять
    std::vector<std::pair<int, int>> moves;

    //мб надо исправить не перебирая все ходы возможные
    for (int x = 0; x < 8; ++x) {
        for (int y = 0; y < 8; ++y) {
            int dx = std::abs(x - pos.first);
            int dy = std::abs(y - pos.second);

            if (dx == dy && dx > 0) {
                moves.push_back({x, y});
            }
        }
    }
    return moves;
}
