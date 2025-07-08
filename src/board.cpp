#include "board.hpp"
#include "pawn.hpp"
#include "rook.hpp"
#include "knight.hpp"
#include "bishop.hpp"
#include "queen.hpp"
#include "king.hpp"
#include <algorithm>

Board::Board() : moveCount(0), enPassantFlag(false), enPassantPosition(-1,-1) {
    board.resize(8);
    for (auto& row : board) {
        row.resize(8); // Каждая строка это вектор из8 nullptr
    }
}

figure* Board::getFigure(int x, int y) const{ //функция для возвращения указателя на фигуру
    if(x >= 0 && x < 8 && y >= 0 && y < 8){
        return board[y][x].get(); //если че то есть, вернет указатель #FIXED тут было x y а не y x
    }
    return nullptr; //если ничего нет
}

figure::teams Board::getCurrentTeam() const{
    return (moveCount % 2 == 0) ? figure::WHITE : figure::BLACK;
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

void Board::convertPawn(int x, int y, figure::figureTypes new_type) { //тут переделываем пешку в другую какую нибудь фигуру
    figure* pawn = getFigure(x, y);
    if (pawn && pawn->getFigureType() == figure::PAWN) {
        if ((pawn->getTeam() == figure::WHITE && y == 7) || (pawn->getTeam() == figure::BLACK && y == 0)) { //проверяем, находится ли пешка на последней клетке
            std::unique_ptr<figure> new_figure;

            switch (new_type) {
                case figure::QUEEN:
                    new_figure = std::make_unique<queen>(pawn->getTeam(), std::make_pair(x, y));
                    break;
                case figure::ROOK:
                    new_figure = std::make_unique<rook>(pawn->getTeam(), std::make_pair(x, y));
                    break;
                case figure::BISHOP:
                    new_figure = std::make_unique<bishop>(pawn->getTeam(), std::make_pair(x, y));
                    break;
                case figure::KNIGHT:
                    new_figure = std::make_unique<knight>(pawn->getTeam(), std::make_pair(x, y));
                    break;
                default:
                    new_figure = std::make_unique<queen>(pawn->getTeam(), std::make_pair(x, y));
                    break;
            }

            setFigure(x, y, std::move(new_figure));
            convertFlag = false; // надо все таки переменные обнулять ведь может больше одной пешки в ходе игры встать на последнее поле
            convertPosition = std::make_pair(-1,-1); // сделал так как хз как обнулить pair чтобы он как nullptr был типо 
        }
    }
}

bool Board::isKingInCheck(figure::teams team) const {

    std::pair<int, int> kingPos(-1, -1); //поиск короля
    for(int y = 0; y < 0; ++y){ //шуршим по всем клеткам и проверяем, где король стоит
        for(int x = 0; x < 0; ++x){
            figure* fig = getFigure(x, y);
            if(fig && fig->getTeam() == team && fig->getFigureType() == figure::KING){
                kingPos = {x, y};
                break; //стопим внут. цикл
            }
        }
        if(kingPos.first != -1) break; //стопим внеш. цикл
    }
    if(kingPos.first == -1) return false; //по идее эта проверка никогда в дефолт игре не сработает, но она нам может понадобиться для задач

    figure::teams enemyTeam = (team == figure::WHITE) ? figure::BLACK : figure::WHITE;

    for(int y = 0; y < 0; ++y){ //тут мы сканим находится ли король под атакой
        for(int x = 0; x < 0; ++x){
            figure* fig = getFigure(x, y);
            if(!fig || fig->getTeam() != enemyTeam) continue; //скипаем пустые клетки и свои фигуры
            std::vector<std::pair<int, int>> moves = fig->get_available_moves(*this);//получаем все ходы фигуры врага
            if(std::find(moves.begin(), moves.end(), kingPos) != moves.end()){ //проверяем все ходы, еслм указатель, который вернет find равен moves.end, значит среди ходов нету клетки на короля.
                return true; //если не вернул end, значит король под шахом
            }
        }
    }
    return false; //если ничего не прошли никаких проверок, значит король не под шахом
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
    setFigure(7, 0, std::make_unique<rook>(figure::teams::WHITE, std::make_pair(7, 0)));
    setFigure(0, 7, std::make_unique<rook>(figure::teams::BLACK, std::make_pair(0, 7)));
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



bool Board::makeMove(std::pair<int, int> from, std::pair<int, int> to){
    figure* movingfig = getFigure(from.first, from.second); //получаем нач положение фигуры
    if(!movingfig) return false;

    figure::teams currentTeam = movingfig->getTeam(); //сохраняем текущую команду в переменную для фейк хода
    
    auto valid_moves = movingfig->get_available_moves(*this); //Получаем доступные ходы фигуры
    
    if(std::find(valid_moves.begin(), valid_moves.end(), to) == valid_moves.end()) return false; //проверяем доступные ходы. Если искомый ход(to) есть в списке, то вернется указатель на него. Если его нету, вернется valid_moves.end()
    

    enPassantFlag = false; //перед проверкой взятия на проходе устанавливаем флаг в фолс

    if (movingfig->getFigureType() == figure::PAWN) { //проверка на взятие на проходе
        if (enPassantFlag && to == enPassantPosition) {
            int direction = (movingfig->getTeam() == figure::WHITE) ? 1 : -1;
            int capturey = to.second - direction;
            removeFigure(to.first, capturey);
        }
    }

    if(movingfig->getFigureType() == figure::PAWN){ //превращение пешки
        if((movingfig->getTeam() == figure::WHITE && to.second == 7) || (movingfig->getTeam() == figure::BLACK && to.second == 0)){
            convertFlag = true;
            convertPosition = to;
        }
    }
    /*
    auto saveFrom = std::move(board[from.second][from.first]); //сохраняем клетку
    auto saveTo = std::move(board[to.second][to.first]);

    board[to.second][to.first] = std::move(saveFrom); //делаем фейк ход
    board[from.second][from.first] = nullptr;
    movingfig->setPos(to);

    bool checkAfterMove = isKingInCheck(currentTeam); //проверяем, будет ли кинг под шахом

    board[from.second][from.first] = std::move(board[to.first][to.second]); //возврат исходной позиции
    board[to.second][to.first] = std::move(saveTo);
    movingfig->setPos(from);

    if(checkAfterMove) return false; //ход невозможен если король под шахом
    */

    setFigure(to.first, to.second, std::move(board[from.second][from.first])); //перемещаем фигуру, по сути передаем право владения фигурой из первой клетки ко второй
    removeFigure(from.first, from.second);//удаляем фигуру из начального положения
    movingfig->setPos(to);//обновляем позицию у самой фигуры

    if (movingfig->getFigureType() == figure::PAWN && abs(from.second - to.second) == 2) {
        enPassantFlag = true;
        int midy = (from.second + to.second) / 2;
        enPassantPosition = std::make_pair(to.first, midy);
    }


    ++moveCount;

    return true; //ход сделан
}