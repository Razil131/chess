#include "../../include/rook.hpp"

rook::rook(teams t, std::pair<int,int> p) {
        team = t;
        pos = p;
        figureType = ROOK;

        if (team == WHITE)
            iconPath = "/materials/rw.png";
        else
            iconPath = "/materials/rb.png";
}

std::vector<std::pair<int, int>> rook::get_available_moves() { //TODO сделать чтобы противники учитывались там передавать доску и проверять
    std::vector<std::pair<int, int>> moves;

    const int directions[4][2] = {
        {+1, 0}, {-1, 0}, {0, +1}, {0, -1}
    };

    for (const auto& d : directions) {
        int nx = pos.first;
        int ny = pos.second;

        while (true) {
            nx += d[0];
            ny += d[1];

            if (nx < 0 || nx > 7 || ny < 0 || ny > 7)
                break;

            moves.push_back({nx, ny});
        }
    }

    return moves;
}
