#include "../../include/knight.hpp"
#include "board.hpp"

knight::knight(teams t, std::pair<int,int> p) {
        team = t;
        pos = p;
        figureType = KNIGHT;

        if (team == WHITE)
            iconPath = "/materials/kw.png";
        else
            iconPath = "/materials/kb.png";
}

std::vector<std::pair<int, int>> knight::get_available_moves(const Board& board) { //TODO сделать чтобы противники учитывались там передавать доску и проверять
    std::vector<std::pair<int, int>> moves;

    const std::vector<std::pair<int, int>> directions = { //все возможные ходы коня
        {+2, +1}, {+2, -1},
        {-2, +1}, {-2, -1},
        {+1, +2}, {+1, -2},
        {-1, +2}, {-1, -2}
    };

    for (auto [dx, dy] : directions) {
        int nx = pos.first + dx;
        int ny = pos.second + dy;

        if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
            if (board.isOccupiedByOwnTeam(nx, ny, team)) 
                continue; //если своя фигура, то дальше не лезем
            moves.emplace_back(nx, ny); //если нету ничего то тоже добавляем
            }
        }
    return moves;
}
