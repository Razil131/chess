#pragma once
#include "figure.hpp"
class pawn:public figure{
    public:
    pawn(teams t, std::pair<int,int> p);
    std::vector<std::pair<int, int>> get_available_moves() override;
};