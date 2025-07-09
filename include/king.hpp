#pragma once
#include "figure.hpp"
#include "board.hpp"
class king:public figure{
    public:
    king(teams t, std::pair<int,int> p, sf::Texture& texture); // доп конструктор только для gui с текстурой
    std::vector<std::pair<int, int>> get_available_moves(const Board& board) override;
};