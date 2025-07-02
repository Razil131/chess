#pragma once
#include "figure.hpp"
class queen:public figure{
    public:
    queen(teams t, std::pair<int,int> p);
    std::vector<std::pair<int, int>> get_available_moves() override;
};