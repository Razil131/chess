#include "../../include/queen.hpp"

queen::queen(teams t, std::pair<int,int> p) {
        team = t;
        pos = p;
        figureType = QUEEN;

        if (team == WHITE)
            iconPath = "/materials/qw.png";
        else
            iconPath = "/materials/qb.png";
}

std::vector<std::pair<int, int>> queen::get_available_moves() { //TODO сделать чтобы противники учитывались там передавать доску и проверять
    std::vector<std::pair<int, int>> moves;

    // направления движения
    const int directions[8][2] = {
        {+1, 0}, {-1, 0}, {0, +1}, {0, -1},
        {+1, +1}, {-1, -1}, {+1, -1}, {-1, +1}
    };

    for (const auto& d : directions) {
        int nx = pos.first;
        int ny = pos.second;

        while (true) {
            nx += d[0];
            ny += d[1];

            if (nx < 0 || nx > 7 || ny < 0 || ny >= 7)
                break;

            moves.push_back({nx, ny});
        }
    }

    return moves;
}
