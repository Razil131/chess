#pragma once
#include "figure.hpp"
#include "board.hpp"

class bishop : public figure { //слон, наследование от класса фигура
public:
    bishop(teams t, std::pair<int,int> p); //конструктор
    std::vector<std::pair<int, int>> get_available_moves(const Board& board) override; //проверка возможных ходов
};
