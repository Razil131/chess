#include "board.hpp"
#include "pawn.hpp"
#include "rook.hpp"
#include "knight.hpp"
#include "bishop.hpp"
#include "queen.hpp"
#include "king.hpp"
#include <algorithm>

Board::Board() {
    board.resize(8);
    for (auto& row : board) {
        row.resize(8); // Каждая строка это вектор из8 nullptr
    }
}

figure* Board::getFigure(int x, int y) const{ //функция для возвращения указателя на фигуру
    if(x >= 0 && x < 8 && y >= 0 && y < 8){
        return board[x][y].get(); //если че то есть, вернет указатель
    }
    return nullptr; //если ничего нет
}

bool Board::isOccupied(int x, int y) const {
    return getFigure(x, y) != nullptr; //если клетка занята, вернется тру, если не занята - фолс
}

bool Board::isOccupiedByOwnTeam(int x, int y, figure::teams team) const{
    figure* fig = getFigure(x, y);
    return fig && fig->getTeam() == team; //если фигура есть и команда совпадает, возращаем тру
}

bool Board::isOccupiedByEnemyTeam(int x, int y, figure::teams team) const{
    figure* fig = getFigure(x, y);
    return fig && fig->getTeam() != team; //если фигура есть и команда не совпадает, возращаем тру
}

void Board::setFigure(int x, int y, std::unique_ptr<figure> fig) {
    if (x >= 0 && x < 8 && y >= 0 && y < 8)
        board[y][x] = std::move(fig); //устанавливает фигуру в нужное место
}

void Board::removeFigure(int x, int y) {
    if (x >= 0 && x < 8 && y >= 0 && y < 8)
        board[y][x].reset(); // Удаляет фигуру, устанавливает nullptr в ее место
}

bool Board::makeMove(std::pair<int, int> from, std::pair<int, int> to){
    figure* movingfig = getFigure(from.first, from.second); //получаем нач положение фигуры
    if(!movingfig) return false;
    
    auto valid_moves = movingfig->get_available_moves(*this); //Получаем доступные ходы фигуры
    
    if(std::find(valid_moves.begin(), valid_moves.end(), to) == valid_moves.end()) return false; //проверяем доступные ходы. Если искомый ход(to) есть в списке, то вернется указатель на него. Если его нету, вернется valid_moves.end()
    
    setFigure(to.first, to.second, std::move(board[from.second][from.first])); //перемещаем фигуру, по сути передаем право владения фигурой из первой клетки ко второй
    
    removeFigure(from.first, from.second);//удаляем фигуру из начального положения

    movingfig->setPos(to);//обновляем позицию у самой фигуры

    return true; //ход сделан
}

void Board::initialize(){ //функция для начального положения фигур, по стандартной схеме
    for(int y = 0; y < 8; ++y){
        for(int x = 0; x < 8; ++x){ //очищаем доску 
            removeFigure(x, y);
        }
    }

    for(int x = 0; x < 8; ++x){
        setFigure(x, 1, std::make_unique<pawn>(figure::teams::WHITE, std::make_pair(x, 1))); //ставим белые пешки
        setFigure(x, 6, std::make_unique<pawn>(figure::teams::BLACK, std::make_pair(x, 6))); //ставим черные пешки
    }
    //ставим ладьи на их места
    setFigure(0, 0, std::make_unique<rook>(figure::teams::WHITE, std::make_pair(0, 0)));
    setFigure(7, 0, std::make_unique<rook>(figure::teams::WHITE, std::make_pair(0, 7)));
    setFigure(0, 7, std::make_unique<rook>(figure::teams::BLACK, std::make_pair(7, 0)));
    setFigure(7, 7, std::make_unique<rook>(figure::teams::BLACK, std::make_pair(7, 7)));

    //ставим коней
    setFigure(1, 0, std::make_unique<knight>(figure::teams::WHITE, std::make_pair(1, 0)));
    setFigure(6, 0, std::make_unique<knight>(figure::teams::WHITE, std::make_pair(6, 0)));
    setFigure(1, 7, std::make_unique<knight>(figure::teams::BLACK, std::make_pair(1, 7)));
    setFigure(6, 7, std::make_unique<knight>(figure::teams::BLACK, std::make_pair(6, 7)));

    //ставим слонов
    setFigure(2, 0, std::make_unique<bishop>(figure::teams::WHITE, std::make_pair(2, 0)));
    setFigure(5, 0, std::make_unique<bishop>(figure::teams::WHITE, std::make_pair(5, 0)));
    setFigure(2, 7, std::make_unique<bishop>(figure::teams::BLACK, std::make_pair(2, 7)));
    setFigure(5, 7, std::make_unique<bishop>(figure::teams::BLACK, std::make_pair(5, 7)));

    //ставим ферзей
    setFigure(3, 0, std::make_unique<queen>(figure::teams::WHITE, std::make_pair(3, 0)));
    setFigure(3, 7, std::make_unique<queen>(figure::teams::BLACK, std::make_pair(3, 7)));

    //ставим королей
    setFigure(4, 0, std::make_unique<king>(figure::teams::WHITE, std::make_pair(4, 0)));
    setFigure(4, 7, std::make_unique<king>(figure::teams::BLACK, std::make_pair(4, 7)));
}