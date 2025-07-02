#pragma once
#include "figure.hpp"
class king:public figure{
    public:
    king(teams t, std::pair<int,int> p);
    std::vector<std::pair<int, int>> get_available_moves() override;
};