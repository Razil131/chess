#pragma once
#include "figure.hpp"

class bishop : public figure {
public:
    bishop(teams t, std::pair<int,int> p);
    std::vector<std::pair<int, int>> get_available_moves() override;
};
