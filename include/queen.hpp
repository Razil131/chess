#pragma once
#include "figure.hpp"
#include "board.hpp"
class queen:public figure{
    public:
    queen(teams t, std::pair<int,int> p);
    std::vector<std::pair<int, int>> get_available_moves(const Board& board) override;
};