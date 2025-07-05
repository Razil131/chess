#include <iostream>
#include "debug_utils.hpp"
#include "board.hpp"

int main() {
    Board board;
    board.initialize();
    printBoard(board);
    std::cout << std::endl;
    board.makeMove(std::make_pair(0, 1), std::make_pair(0, 2));
    printBoard(board);
    std::cout << std::endl;
    board.makeMove(std::make_pair(0, 0), std::make_pair(0, 1));
    printBoard(board);
    std::cout << std::endl;
    board.makeMove(std::make_pair(1, 0), std::make_pair(1, 2));
    printBoard(board);
}