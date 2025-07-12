#include "king.hpp"
#include "board.hpp"


king::king(teams t, std::pair<int,int> p,sf::Texture& texture) { // доп конструктор только для gui с текстурой
        team = t;
        pos = p;
        figureType = KING;
        hasMoved = false;

        iconPath = (team == WHITE) ? "/materials/kw.png" : "/materials/kb.png";

        sprite.setTexture(texture);
        sprite.setScale(1.2f, 1.2f);
}

std::vector<std::pair<int, int>> king::get_available_moves(const Board& board) { 
    std::vector<std::pair<int, int>> moves;

    // направления движения
    const std::vector<std::pair<int, int>> directions = {
        {+1, 0}, {-1, 0}, {0, +1}, {0, -1},
        {+1, +1}, {-1, -1}, {+1, -1}, {-1, +1}
    };

    for (auto [dx, dy] : directions) {
        int nx = pos.first + dx; //первый ход по иксу
        int ny = pos.second + dy; //первый ход по игрику
        if (nx >= 0 && nx < 8 && ny >= 0 && ny < 8) {
            if (board.isOccupiedByOwnTeam(nx, ny, team)) continue; //если своя фигура, то нельзя туда идти
                    moves.emplace_back(nx, ny);
                }
    }
    return moves;
}
