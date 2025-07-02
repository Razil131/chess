#pragma once
#include "figure.hpp"
class rook:public figure{
    public:
    rook(teams t, std::pair<int,int> p);
    std::vector<std::pair<int, int>> get_available_moves() override;
};