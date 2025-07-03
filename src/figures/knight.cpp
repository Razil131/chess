#include "../../include/knight.hpp"

knight::knight(teams t, std::pair<int,int> p) {
        team = t;
        pos = p;
        figureType = KNIGHT;

        if (team == WHITE)
            iconPath = "/materials/kw.png";
        else
            iconPath = "/materials/kb.png";
}

std::vector<std::pair<int, int>> knight::get_available_moves() { //TODO сделать чтобы противники учитывались там передавать доску и проверять
    std::vector<std::pair<int, int>> moves;

    const int offsets[8][2] = { //все возможные ходы коня
        {+2, +1}, {+2, -1},
        {-2, +1}, {-2, -1},
        {+1, +2}, {+1, -2},
        {-1, +2}, {-1, -2}
    };

    for (const auto& o : offsets) {
        int nx = pos.first + o[0];
        int ny = pos.second + o[1];

        if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
            moves.push_back({nx, ny});
        }
    }

    return moves;
}
