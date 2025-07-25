#include "pawn.hpp"
#include "board.hpp"

pawn::pawn(teams t, std::pair<int,int> p,sf::Texture& texture) { // доп конструктор только для gui с текстурой
        team = t;
        pos = p;
        figureType = PAWN;

        iconPath = (team == WHITE) ? "/materials/pw.png" : "/materials/pb.png";

        sprite.setTexture(texture);
        sprite.setScale(0.15f, 0.15f);
}

std::vector<std::pair<int, int>> pawn::get_available_moves(const Board& board) {
    std::vector<std::pair<int, int>> moves;

    int direction = (team == WHITE) ? +1 : -1;

    int nx = pos.first;
    int ny = pos.second + direction;

    if (ny >= 0 && ny < 8) { //чисто ходить прямо
        if (!board.isOccupied(nx, ny)) {
            moves.emplace_back(nx, ny);
        }
    }

    if ((team == WHITE && pos.second == 1) || (team == BLACK && pos.second == 6)) { //если на начальной клетке стоит, то может ходить на 2 клетки
        int ny_one_step = pos.second + direction; //на одну клетку
        int ny_two_steps = pos.second + 2 * direction; //на две

        if (!board.isOccupied(nx, ny_one_step) && !board.isOccupied(nx, ny_two_steps) && ny_two_steps >= 0 && ny_two_steps < 8) {
            moves.emplace_back(nx, ny_two_steps); //проверяем все условия и если можем, идем шагаем на две клетки
        }
    }
    for(int dx : {-1, +1}){ 
            int diag_x = pos.first + dx;
            int diag_y = pos.second + direction;

            if(diag_x < 0 || diag_x >= 8 || diag_y < 0 || diag_y >= 8) continue;

            if (board.isOccupiedByEnemyTeam(diag_x, diag_y, team)) { //Тут взятие по диагонали сделано
                    moves.emplace_back(diag_x, diag_y);
                }
            else if (board.enPassantFlag && board.enPassantPosition == std::make_pair(diag_x, diag_y)) { //взятие на проходе
            moves.emplace_back(diag_x, diag_y);
        }
            }
    return moves;
}

