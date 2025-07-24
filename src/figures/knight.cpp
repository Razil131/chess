#include "knight.hpp"
#include "board.hpp"

knight::knight(teams t, std::pair<int,int> p,sf::Texture& texture) { // доп конструктор только для gui с текстурой
        team = t;
        pos = p;
        figureType = KNIGHT;

        iconPath = (team == WHITE) ? "/materials/nw.png" : "/materials/nb.png";

        sprite.setTexture(texture);
        sprite.setScale(0.15f, 0.15f);
}

std::vector<std::pair<int, int>> knight::get_available_moves(const Board& board) {
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
