#include "board.hpp"
#include "pawn.hpp"
#include "rook.hpp"
#include "knight.hpp"
#include "bishop.hpp"
#include "queen.hpp"
#include "king.hpp"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>
#include <iostream>
#include <random>

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

// надоело мне уже это first second
figure* Board::getFigure(std::pair<int,int> pos) const{ //функция для возвращения указателя на фигуру
    int x = pos.first;
    int y = pos.second;
    if(x >= 0 && x < 8 && y >= 0 && y < 8){
        return board[y][x].get(); //если че то есть, вернет указатель #FIXED тут было x y а не y x
    }
    return nullptr; //если ничего нет
}

// чтобы gui реально отображало что происходит на бэкенд
std::vector<std::vector<std::unique_ptr<figure>>>* Board::getBoard(){
    return &board;
}

// получить чей сейчас ход
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

void Board::convertPawn(int x, int y, figure::figureTypes new_type, sf::Texture& texture) { //тут переделываем пешку в другую какую нибудь фигуру
    figure* pawn = getFigure(x, y);
    if (pawn && pawn->getFigureType() == figure::PAWN) {
        if ((pawn->getTeam() == figure::WHITE && y == 7) || (pawn->getTeam() == figure::BLACK && y == 0)) { //проверяем, находится ли пешка на последней клетке
            std::unique_ptr<figure> new_figure;

            switch (new_type) {
                case figure::QUEEN:
                    new_figure = std::make_unique<queen>(pawn->getTeam(), std::make_pair(x, y), texture);
                    break;
                case figure::ROOK:
                    new_figure = std::make_unique<rook>(pawn->getTeam(), std::make_pair(x, y), texture);
                    break;
                case figure::BISHOP:
                    new_figure = std::make_unique<bishop>(pawn->getTeam(), std::make_pair(x, y), texture);
                    break;
                case figure::KNIGHT:
                    new_figure = std::make_unique<knight>(pawn->getTeam(), std::make_pair(x, y), texture);
                    break;
                default:
                    new_figure = std::make_unique<queen>(pawn->getTeam(), std::make_pair(x, y), texture);
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
    for(int y = 0; y < 8; ++y){ //шуршим по всем клеткам и проверяем, где король стоит
        for(int x = 0; x < 8; ++x){
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

    for(int y = 0; y < 8; ++y){ //тут мы сканим находится ли король под атакой
        for(int x = 0; x < 8; ++x){
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

void Board::initialize(std::map<std::string, sf::Texture>& textures){ //функция для начального положения фигур, по стандартной схеме
    whiteCanCastleKingSide  = whiteCanCastleQueenSide  = blackCanCastleKingSide  = blackCanCastleQueenSide  = true;
    updateFen();
    moveCount = 0; // чтобы при перезапуске игры игра не начиналась с хода черных
    for(int y = 0; y < 8; ++y){
        for(int x = 0; x < 8; ++x){ //очищаем доску 
            removeFigure(x, y);
        }
    }

    for(int x = 0; x < 8; ++x){
        setFigure(x, 1, std::make_unique<pawn>(figure::teams::WHITE, std::make_pair(x, 1), textures["pw"])); //ставим белые пешки
        setFigure(x, 6, std::make_unique<pawn>(figure::teams::BLACK, std::make_pair(x, 6), textures["pb"])); //ставим черные пешки
    }
    //ставим ладьи на их места
    setFigure(0, 0, std::make_unique<rook>(figure::teams::WHITE, std::make_pair(0, 0), textures["rw"]));
    setFigure(7, 0, std::make_unique<rook>(figure::teams::WHITE, std::make_pair(7, 0), textures["rw"]));
    setFigure(0, 7, std::make_unique<rook>(figure::teams::BLACK, std::make_pair(0, 7), textures["rb"]));
    setFigure(7, 7, std::make_unique<rook>(figure::teams::BLACK, std::make_pair(7, 7), textures["rb"]));

    //ставим коней
    setFigure(1, 0, std::make_unique<knight>(figure::teams::WHITE, std::make_pair(1, 0), textures["nw"]));
    setFigure(6, 0, std::make_unique<knight>(figure::teams::WHITE, std::make_pair(6, 0), textures["nw"]));
    setFigure(1, 7, std::make_unique<knight>(figure::teams::BLACK, std::make_pair(1, 7), textures["nb"]));
    setFigure(6, 7, std::make_unique<knight>(figure::teams::BLACK, std::make_pair(6, 7), textures["nb"]));

    //ставим слонов
    setFigure(2, 0, std::make_unique<bishop>(figure::teams::WHITE, std::make_pair(2, 0), textures["bw"]));
    setFigure(5, 0, std::make_unique<bishop>(figure::teams::WHITE, std::make_pair(5, 0), textures["bw"]));
    setFigure(2, 7, std::make_unique<bishop>(figure::teams::BLACK, std::make_pair(2, 7), textures["bb"]));
    setFigure(5, 7, std::make_unique<bishop>(figure::teams::BLACK, std::make_pair(5, 7), textures["bb"]));

    //ставим ферзей
    setFigure(3, 0, std::make_unique<queen>(figure::teams::WHITE, std::make_pair(3, 0), textures["qw"]));
    setFigure(3, 7, std::make_unique<queen>(figure::teams::BLACK, std::make_pair(3, 7), textures["qb"]));

    //ставим королей
    setFigure(4, 0, std::make_unique<king>(figure::teams::WHITE, std::make_pair(4, 0), textures["kw"]));
    setFigure(4, 7, std::make_unique<king>(figure::teams::BLACK, std::make_pair(4, 7), textures["kb"]));
}

void Board::fisherPos(std::map<std::string, sf::Texture>& textures){ //функция расстановки по фишеру
    for(int y = 0; y < 8; ++y){
        for(int x = 0; x < 8; ++x){ //очищаем доску 
            removeFigure(x, y);
        }
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::vector<int> numbers = {0, 1, 2, 3, 4, 5, 6, 7}; //вектор клеток
    std::vector<int> evens = {0, 2, 4, 6}; //четные
    std::vector<int> odds = {1, 3, 5, 7}; //нечетные

    std::shuffle(numbers.begin(), numbers.end(), gen); //перемешиваем
    std::shuffle(evens.begin(), evens.end(), gen); //перемешиваем
    std::shuffle(odds.begin(), odds.end(), gen); //перемешиваем

    std::vector<int> res;

    res.push_back(evens[0]); //берем четное и нечетное число чтобы сначала поставить слонов
    res.push_back(odds[0]);

    auto it1 = std::find(numbers.begin(), numbers.end(), evens[0]); //находим эти клетки и убираем их в numbers
    auto it2 = std::find(numbers.begin(), numbers.end(), odds[0]);
    if (it1 < it2) {
        numbers.erase(it2);
        numbers.erase(it1);
    } else {
        numbers.erase(it1);
        numbers.erase(it2);
    }
    auto maxit = std::max_element(numbers.begin(), numbers.end()); //находим максимальный элемент
    auto minit = std::min_element(numbers.begin(), numbers.end());
    int min = *minit;
    int max = *maxit;

    for(int i = 0; i < numbers.size(); i++){ //цикл для того, чтобы не поставить короля на крайнюю клетку, ибо от него справа и слева должны стоять ладьи
        if(numbers[i] != min && numbers[i] != max){
            res.push_back(numbers[i]);
            numbers.erase(numbers.begin() + i);
            break;
        }
        else continue;
    }
    
    int kingPos = res[2];
    std::vector<int> less, greater;
    for (int v : numbers){
        if(v < kingPos) less.push_back(v);
        else if (v > kingPos) greater.push_back(v);
    }

    std::shuffle(less.begin(), less.end(), gen);
    std::shuffle(greater.begin(), greater.end(), gen);

    int rookL = less.front();
    int rookR = greater.front();
    whiteRookKS = blackRookKS = rookR;
    whiteRookQS = blackRookQS = rookL;

    numbers.erase(std::find(numbers.begin(), numbers.end(), rookL));
    numbers.erase(std::find(numbers.begin(), numbers.end(), rookR));

    res.push_back(rookL);
    res.push_back(rookR);

    for(int i = 0; i<numbers.size(); i++){
        res.push_back(numbers[i]);
    }


    for(int x = 0; x < 8; ++x){
        setFigure(x, 1, std::make_unique<pawn>(figure::teams::WHITE, std::make_pair(x, 1), textures["pw"])); //ставим белые пешки
        setFigure(x, 6, std::make_unique<pawn>(figure::teams::BLACK, std::make_pair(x, 6), textures["pb"])); //ставим черные пешки
    }
    //ставим ладьи на их места
    setFigure(res[3], 0, std::make_unique<rook>(figure::teams::WHITE, std::make_pair(res[3], 0), textures["rw"]));
    setFigure(res[4], 0, std::make_unique<rook>(figure::teams::WHITE, std::make_pair(res[4], 0), textures["rw"]));
    setFigure(res[3], 7, std::make_unique<rook>(figure::teams::BLACK, std::make_pair(res[3], 7), textures["rb"]));
    setFigure(res[4], 7, std::make_unique<rook>(figure::teams::BLACK, std::make_pair(res[4], 7), textures["rb"]));

    //ставим коней
    setFigure(res[5], 0, std::make_unique<knight>(figure::teams::WHITE, std::make_pair(res[5], 0), textures["nw"]));
    setFigure(res[6], 0, std::make_unique<knight>(figure::teams::WHITE, std::make_pair(res[6], 0), textures["nw"]));
    setFigure(res[5], 7, std::make_unique<knight>(figure::teams::BLACK, std::make_pair(res[5], 7), textures["nb"]));
    setFigure(res[6], 7, std::make_unique<knight>(figure::teams::BLACK, std::make_pair(res[6], 7), textures["nb"]));

    //ставим слонов
    setFigure(res[0], 0, std::make_unique<bishop>(figure::teams::WHITE, std::make_pair(res[0], 0), textures["bw"]));
    setFigure(res[1], 0, std::make_unique<bishop>(figure::teams::WHITE, std::make_pair(res[1], 0), textures["bw"]));
    setFigure(res[0], 7, std::make_unique<bishop>(figure::teams::BLACK, std::make_pair(res[0], 7), textures["bb"]));
    setFigure(res[1], 7, std::make_unique<bishop>(figure::teams::BLACK, std::make_pair(res[1], 7), textures["bb"]));

    //ставим ферзей
    setFigure(res[7], 0, std::make_unique<queen>(figure::teams::WHITE, std::make_pair(res[7], 0), textures["qw"]));
    setFigure(res[7], 7, std::make_unique<queen>(figure::teams::BLACK, std::make_pair(res[7], 7), textures["qb"]));

    // //ставим королей
    setFigure(res[2], 0, std::make_unique<king>(figure::teams::WHITE, std::make_pair(res[2], 0), textures["kw"]));
    setFigure(res[2], 7, std::make_unique<king>(figure::teams::BLACK, std::make_pair(res[2], 7), textures["kb"]));

    //генерируем строку для позиции
    std::ostringstream fenStream;
    
    for(int y = 7; y >= 0; --y){ //перебираем ряды
        int emptycount = 0;
        for(int x = 0; x < 8; ++x){
            figure* fig = getFigure(x, y);
            if(!fig){
                ++emptycount;
            } else{
                if(emptycount){
                    fenStream << emptycount;
                    emptycount = 0;
                }
                char c;
                switch (fig->getFigureType()){
                    case figure::PAWN: c = 'P'; break;
                    case figure::KNIGHT: c = 'N'; break;
                    case figure::BISHOP: c = 'B'; break;
                    case figure::ROOK: c = 'R'; break;
                    case figure::QUEEN: c = 'Q'; break;
                    case figure::KING: c = 'K'; break;
                    default: c = '?'; break;
                }
                if(fig->getTeam() == figure::BLACK)
                c = std::tolower(c);
                fenStream << c;
            }
        }
        if(emptycount) fenStream << emptycount;
        if(y>0) fenStream << '/';
    }
    fenStream << " w KQkq - 0 1";
    this-> fenPos = fenStream.str();
}

void Board::updateFen() { //функция для обновления фен позиции
    std::ostringstream fen;
    for(int y = 7; y >= 0; --y){ //записываем все в строчки
        int empty = 0;
        for(int x = 0; x < 8; ++x){
            if (figure* f = getFigure(x, y)) {
                if (empty) { fen << empty; empty = 0; }
                char c;
                switch (f->getFigureType()) {
                    case figure::PAWN:   c = 'P'; break;
                    case figure::KNIGHT: c = 'N'; break;
                    case figure::BISHOP: c = 'B'; break;
                    case figure::ROOK:   c = 'R'; break;
                    case figure::QUEEN:  c = 'Q'; break;
                    case figure::KING:   c = 'K'; break;
                    default:             c = '?'; break;
                }
                fen << (f->getTeam()==figure::BLACK ? char(std::tolower(c)) : c);
            } else {
                ++empty;
            }
        }
        if (empty) fen << empty;
        if (y)     fen << '/';
    }
    fen << (getCurrentTeam()==figure::WHITE ? " w " : " b "); //определяем чей ход
    std::string castling;
    if (whiteCanCastleKingSide)   castling += 'K'; //заполняем рокировку
    if (whiteCanCastleQueenSide)  castling += 'Q';
    if (blackCanCastleKingSide)   castling += 'k';
    if (blackCanCastleQueenSide)  castling += 'q';
    if (castling.empty()) castling = "-";

    fen << castling << ' ';

    if (enPassantFlag) { //заполняем взятие на проходе
        int ex = enPassantPosition.first;
        int ey = enPassantPosition.second;
        fen << char('a'+ex) << char('1'+ey) << ' ';
    } else {
        fen << "- ";
    }
    
    fen << "0 " << (moveCount/2 + 1); //полупроходы и счётчик ходов можно просто обнулять или считать по moveCount хз надо это или нет но вроде это стандарт
    this->fenPos = fen.str();
}


bool Board::makeMove(std::pair<int, int> from, std::pair<int, int> to){
    figure* movingfig = getFigure(from.first, from.second); //получаем нач положение фигуры
    if(!movingfig) return false;

    mateFlag = false;
    staleMateFlag = false;

    bool prevEnPassantFlag = enPassantFlag;
    std::pair<int, int> prevEnPassantPosition = enPassantPosition; //по идее это должно пофиксить то что фигура не пропадала
    
    auto valid_moves = getValidMoves(from.first, from.second); //Получаем доступные ходы фигуры
    
    if(std::find(valid_moves.begin(), valid_moves.end(), to) == valid_moves.end()) return false; //проверяем доступные ходы. Если искомый ход(to) есть в списке, то вернется указатель на него. Если его нету, вернется valid_moves.end()


    auto saveFrom = std::move(board[from.second][from.first]); // Сохраняем текущее состояние если ход приведёт к шаху
    auto saveTo = std::move(board[to.second][to.first]);

    board[to.second][to.first] = std::move(saveFrom); // Выполняем фейк ход для проверки шаха
    board[from.second][from.first] = nullptr;
    movingfig->setPos(to);

    if (isKingInCheck(movingfig->getTeam())) { 
        board[from.second][from.first] = std::move(board[to.second][to.first]);
        board[to.second][to.first] = std::move(saveTo);
        movingfig->setPos(from);
        return false;
    }

    movingfig->setMoved(true); //метим фигуру ходившей

    if(movingfig->getFigureType() == figure::KING){ //рокируем
        int dx = to.first - from.first;
        if(abs(dx) == 2){
            bool kingSide = (dx > 0);
            int rookFromX = kingSide ? 7 : 0; 
            int rookToX = kingSide ? to.first - 1 : to.first + 1;
            int y = from.second;

            if(figure* rook = getFigure(rookFromX, y)){
                rook->setMoved(true);
                board[y][rookToX] = std::move(board[y][rookFromX]);
                rook->setPos({rookToX, y});
            }
        }
    }

    enPassantFlag = false;
    enPassantPosition = {-1, -1}; //сбрасываем


    if (movingfig->getFigureType() == figure::PAWN && abs(from.second - to.second) == 2) {
        enPassantFlag = true;
        int midy = (from.second + to.second) / 2;
        enPassantPosition = std::make_pair(to.first, midy);
    }

    if (movingfig->getFigureType() == figure::PAWN) { //проверка на взятие на проходе
        if (prevEnPassantFlag && to == prevEnPassantPosition) {
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

    figure::teams opTeam = (movingfig->getTeam() == figure::WHITE ? figure::BLACK : figure::WHITE);

    mateFlag = isKingInMate(opTeam);
    staleMateFlag = isKingInStalemate(opTeam);

    ++moveCount;

    std::string uci = {char('a' + from.first), char('1' + from.second), char('a' + to.first), char('1' + to.second)}; //делаем строку формата а1а1 для хода, такие принимает движок
    if (movingfig->getFigureType() == figure::PAWN && //проверяем для бота превращение пешки
    ((movingfig->getTeam() == figure::WHITE && to.second == 7) ||
     (movingfig->getTeam() == figure::BLACK && to.second == 0)))
    {

    char promo = 'q';
    uci.push_back(promo);
    }
    movesUCI.push_back(uci);

    if (movingfig->getFigureType() == figure::KING) { //сбрасываем флаги рокировки
    if (movingfig->getTeam() == figure::WHITE) {
        whiteCanCastleKingSide  = false;
        whiteCanCastleQueenSide = false;
    } else {
        blackCanCastleKingSide  = false;
        blackCanCastleQueenSide = false;
    }
}

    if (movingfig->getFigureType() == figure::ROOK) {
    int fx = from.first;
    int fy = from.second;
    if (movingfig->getTeam() == figure::WHITE) {
        if (fx == whiteRookKS && fy == 0) whiteCanCastleKingSide  = false;
        if (fx == whiteRookQS && fy == 0) whiteCanCastleQueenSide = false;
    } else {
        if (fx == blackRookKS && fy == 7) blackCanCastleKingSide  = false;
        if (fx == blackRookQS && fy == 7) blackCanCastleQueenSide = false;
    }
}

    updateFen();
    return true; //ход сделан
    }

bool Board::wouldKingInCheck(std::pair<int, int> from, std::pair<int, int> to){ //метод для того, чтобы проверять будт ли король под шахом, делая фейк ход и проверяя, король под шахом или нет
    figure* movingFig = getFigure(from.first, from.second);
    if (!movingFig) return false;

    auto tempFrom = std::move(board[from.second][from.first]); //сохраняем временно состояние для фейк хода
    auto tempTo = std::move(board[to.second][to.first]);
    figure::teams team = movingFig->getTeam();
    bool prevEnPassantFlag = enPassantFlag;
    std::pair<int, int> prevEnPassantPosition = enPassantPosition;

    board[to.second][to.first] = std::move(tempFrom); //перемещаем
    board[from.second][from.first] = nullptr;
    movingFig->setPos(to);

    bool inCheck = isKingInCheck(team); //проверяем, будет ли король под шахом после этого хода

    board[from.second][from.first] = std::move(board[to.second][to.first]); //возвращаем
    board[to.second][to.first] = std::move(tempTo);
    movingFig->setPos(from);
    enPassantFlag = prevEnPassantFlag;
    enPassantPosition = prevEnPassantPosition;

    return inCheck;
}

std::vector<std::pair<int, int>> Board::getValidMoves(int x, int y) { //используется теперь вместо get_availible_moves, вызывает его в себе. Показывает только РЕАЛЬНО доступные ходы, а не все возможные по правилам, как было раньше
    figure* fig = getFigure(x, y);
    if (!fig) return {};
    
    std::vector<std::pair<int, int>> validMoves;
    auto moves = fig->get_available_moves(*this);
    
    for (const auto& move : moves) { //получаем все ходы и проверяем, какие из них не ведут к тому, что король будет под шахом. Если такие есть, их не добавляем
        if(fig->getFigureType() == figure::KING){ //проверяем будет ли клетка для рокировки под шахом
            int dx = move.first - x;
            if(abs(dx) == 2){
                int step = (dx > 0) ? 1 : -1;
                std::pair<int, int> inter(x + step, y);

                if(wouldKingInCheck({x, y}, inter)) continue;
                
            }
        }

        if (!wouldKingInCheck({x, y}, move)) {
            validMoves.push_back(move);
        }
    }

    if(fig->getFigureType() == figure::KING && !fig->getMoved() && !isKingInCheck(fig->getTeam())){ //если это король, он не ходил и не под шахом щас
        if(figure* rook = getFigure(7, y); rook && rook->getFigureType() == figure::ROOK && rook->getTeam() == fig->getTeam() && !rook->getMoved()){ //короткая рокировка
            if(!isOccupied(5, y) && !isOccupied(6, y)){
                if(!isSquareAttack({4, y}, fig->getTeam()) && !isSquareAttack({5, y}, fig->getTeam()) && !isSquareAttack({6, y}, fig->getTeam())){
                    validMoves.emplace_back(6, y);
                }
            }
        }
        if(figure* rook = getFigure(0, y); rook && rook->getFigureType() == figure::ROOK && rook->getTeam() == fig->getTeam() && !rook->getMoved()){ //длинная рокировка
            if(!isOccupied(1, y) && !isOccupied(2, y) && !isOccupied(3, y)){
                if(!isSquareAttack({4, y}, fig->getTeam()) && !isSquareAttack({3, y}, fig->getTeam()) && !isSquareAttack({2, y}, fig->getTeam())){
                    validMoves.emplace_back(2, y);
                }
            }
        }
    }

    return validMoves;
}

bool Board::isKingInMate(figure::teams team) { //проверяем был ли мат, в функции мы смотрим есть ли ходы у фигуры команды, которой шах поставили
    if(!isKingInCheck(team)) return false;
    for(int y = 0; y < 8; ++y){
        for(int x = 0; x < 8; ++x){
            figure* fig = getFigure(x, y);
            if(!fig || fig->getTeam() != team) continue; //скипаем пустые клетки и чужие фигуры
            std::vector<std::pair<int, int>> currentFigMoves = getValidMoves(x, y);
            if(!currentFigMoves.empty()){
                return false;
            }
        }
    }
    return true;
}

bool Board::isKingInStalemate(figure::teams team) { //проверяем был ли пат, в функции мы смотрим есть ли ходы у фигуры команды которая некст ходит
    if(isKingInCheck(team)) return false;
    for(int y = 0; y < 8; ++y){
        for(int x = 0; x < 8; ++x){
            figure* fig = getFigure(x, y);
            if(!fig || fig->getTeam() != team) continue; //скипаем пустые клетки и чужие фигуры
            std::vector<std::pair<int, int>> currentFigMoves = getValidMoves(x, y);
            if(!currentFigMoves.empty()){
                return false;
            }
        }
    }
    return true;
}


bool Board::isSquareAttack(std::pair<int, int> square, figure::teams team) const{ //првоерка клетки на атаку
    figure::teams opTeam = (team == figure::WHITE) ? figure::BLACK : figure::WHITE;

    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            figure* fig = getFigure(x, y);
            if (!fig || fig->getTeam() != opTeam) continue;
            auto moves = fig->get_available_moves(*this);
            if(std::find(moves.begin(), moves.end(), square) != moves.end()){
                return true;
            }
        }
    }
    return false;
}



bool Board::fisherCastle(bool kingSide){ //функция для рокировки по фишеру
    figure::teams team = getCurrentTeam(); //получаем команду
    int gor = (team == figure::WHITE ? 0 : 7); //получаем горизонталь

    int kingX = -1;
    for(int x = 0; x < 8; ++x){ //ищем короля
        figure* f = getFigure(x, gor);
        if(f && f->getFigureType() == figure::KING && f->getTeam() == team){
            kingX = x;
            break;
        }
    }

    figure* king = getFigure(kingX, gor);

    if(king->getMoved() || isKingInCheck(team)){ //проверяем, шевелился ли он и под шахом ли он
        return false;
    }

    int rookX = -1;
    if(kingSide){ //ищем ладью
        for(int x = kingX + 1; x<8; ++x){
            figure* f = getFigure(x, gor);
            if (f && f->getFigureType() == figure::ROOK && f->getTeam() == team) {
                rookX = x;
                break;
                }
            }
    
        }
    else {
        for(int x = kingX - 1; x>=0; --x){
            figure* f = getFigure(x, gor);
            if (f && f->getFigureType() == figure::ROOK && f->getTeam() == team) {
                rookX = x;
                break;
                }
            }
    
        }
    if(rookX < 0) return false;
    figure* rook = getFigure(rookX, gor);

    if(rook->getMoved()) return false; //если ладья ходила, то фолс

    int step = (rookX > kingX ? +1 : -1);
    for(int x = kingX + step; x != rookX; x += step){ //проверяем, есть кто то между или нет
        if (isOccupied(x, gor)) return false;
    }

    int kingToX = kingSide ? 6 : 2; //проверяем, атакуются ли клетки для перехода
    for(int x = kingX; ; x += (kingToX > kingX ? +1 : -1)){
        if(isSquareAttack({x, gor}, team)) return false;
        if(x == kingToX) break;
    }

    int rookToX = kingSide ? 5 : 3;

    if ((kingToX != kingX && isOccupied(kingToX, gor)) || (rookToX != rookX && isOccupied(rookToX, gor))) {
       return false;
    }

    std::unique_ptr<figure> kingPtr, rookPtr;
    if (kingToX != kingX) kingPtr = std::move(board[gor][kingX]);
    if (rookToX != rookX) rookPtr = std::move(board[gor][rookX]);

    if (kingToX != kingX) board[gor][kingToX] = std::move(kingPtr);
    if (rookToX != rookX) board[gor][rookToX] = std::move(rookPtr);

    if (kingToX != kingX) board[gor][kingX].reset();
    if (rookToX != rookX) board[gor][rookX].reset();


    king->setPos({kingToX, gor});
    rook->setPos({rookToX, gor});

    king->setMoved(true);
    rook->setMoved(true);

    std::string move;
    move += static_cast<char>('a' + kingX);
    move += static_cast<char>('1' + gor);
    move += static_cast<char>('a' + kingToX);
    move += static_cast<char>('1' + gor);
    
    movesUCI.push_back(move);
    
    moveCount++;
    if (team == figure::WHITE) {
        if (kingSide)  whiteCanCastleKingSide  = false;
        else           whiteCanCastleQueenSide = false;
    } else {
        if (kingSide)  blackCanCastleKingSide  = false;
        else           blackCanCastleQueenSide = false;
    }
    
    updateFen();
    return true;
}

bool Board::exportToFile(const std::string& filename, int players, int mode){ // функция для создания сохранения
    updateFen(); //обновляем позицию

    std::filesystem::path saveDir = std::filesystem::current_path().parent_path() / "saves"; //составляем путь до сейв папки

    std::filesystem::create_directories(saveDir);

    std::filesystem::path fullpath = saveDir / filename;

    std::ofstream out(fullpath); //открываем файл для записи
    if(!out.is_open()) return false; //если не открылось

    out << players << "\n" << mode << "\n"; //записываем количество игроков и режим
    out << fenPos << "\n"; //записываем позицию
    
    return false;
} 

bool Board::importFromFile(const std::string& filename,  std::map<std::string, sf::Texture>& textures){
 
    std::filesystem::path saveDir = std::filesystem::current_path().parent_path() / "saves"; //составляем путь до сейв папки
    std::filesystem::path fullpath = saveDir / filename;
    int players,mode;
    std::ifstream in(fullpath);
    if(!in.is_open()) return false;

    std::string line, fen;

    if (!std::getline(in, line)) return false; //читаем строки, чтобы получить количество игроков, режим и фен расстановку
    players = std::stoi(line);
    if (!std::getline(in, line)) return false;
    mode   = std::stoi(line);
    if (!std::getline(in, fen))  return false;
    in.close();

    //тут мы будем делить фен строку на части: расположение, сторона, роккировка, взятие на проходе, ходы и полуходы
    std::istringstream ss(fen);
    std::string placement, side, castling, ep, hmove, fmove;
    if (!(ss >> placement >> side >> castling >> ep >> hmove >> fmove)) return false;


    for(int y=0; y<8; ++y) //очищаем доску на всякий случай
        for(int x=0; x<8; ++x)
            removeFigure(x,y);

    //восстанавливаем фигуры из фен
    int x = 0, y = 7;
    for(char c : placement){
        if(c == '/'){
            --y; x = 0;
        }
        else if(std::isdigit(c)){
            x += c - '0';
        }
        else{
            figure::teams team = std::isupper(c) ? figure::WHITE : figure::BLACK;
            char lower = std::tolower(c);
            std::string key = std::string(1, lower) + (team == figure::WHITE ? "w" : "b"); // FIXED была точка с запятой посередине строки
            std::unique_ptr<figure> fig;
            switch(lower){
                case 'p': fig = std::make_unique<pawn>(  team, std::pair{x,y}, textures[key]); break;
                case 'n': fig = std::make_unique<knight>(team, std::pair{x,y}, textures[key]); break;
                case 'b': fig = std::make_unique<bishop>(team, std::pair{x,y}, textures[key]); break;
                case 'r': fig = std::make_unique<rook>(  team, std::pair{x,y}, textures[key]); break;
                case 'q': fig = std::make_unique<queen>( team, std::pair{x,y}, textures[key]); break;
                case 'k': fig = std::make_unique<king>(  team, std::pair{x,y}, textures[key]); break;
                default:  break;
            }
            if(fig) setFigure(x, y, std::move(fig));
            ++x;
        }
    }

    bool whiteToMove = (side == "w"); //узнаем чья очередь ходить
    int fm = std::stoi(fmove);
    moveCount = whiteToMove ? (fm - 1)*2 : (fm - 1)*2 + 1;
        
    whiteCanCastleKingSide = (castling.find('K') != std::string::npos); //раздаем права на рокировку
    whiteCanCastleQueenSide = (castling.find('Q') != std::string::npos);
    blackCanCastleKingSide = (castling.find('k') != std::string::npos);
    blackCanCastleQueenSide = (castling.find('q') != std::string::npos);
    
    //узнаем про взятие на проходе
    if (ep != "-") {
        enPassantFlag = true;
        enPassantPosition = { ep[0] - 'a', ep[1] - '1' };
    } else {
        enPassantFlag = false;
        enPassantPosition = {-1,-1};
    }

    updateFen();
    return true;

}

void Board::clear(){
    for(int y = 0; y < 8; ++y){
        for(int x = 0; x < 8; ++x){ //очищаем доску 
            removeFigure(x, y);
        }
    }
}

bool Board::logFen(const std::string& filename) const{ 
    const_cast<Board*>(this)->updateFen();
    
    std::filesystem::path saveDir = std::filesystem::current_path().parent_path() / "puzzles"; //составляем путь до сейв папки

    std::filesystem::create_directories(saveDir);

    std::filesystem::path fullpath = saveDir / filename;

    std::ofstream out(fullpath, std::ios::app);

    if (!out.is_open()) {
            return false;
        }

    out << fenPos << "\n";
    return true;
}

bool Board::loadPosFromFEN(const std::string& fen, std::map<std::string, sf::Texture>& textures){ //загружаем одну строку
    std::istringstream ss(fen); //парсим поля
    std::string placement, side, castling, ep, hmove, fmove;
    if (!(ss >> placement >> side >> castling >> ep >> hmove >> fmove))
        return false;

    for(int y=0; y<8; ++y){
        for(int x=0; x<8; ++x){
            removeFigure(x,y);
        }
    }

    //восстанавливаем фигуры из фен
    int x = 0, y = 7;
    for(char c : placement){
        if(c == '/'){
            --y; x = 0;
        }
        else if(std::isdigit(c)){
            x += c - '0';
        }
        else{
            figure::teams team = std::isupper(c) ? figure::WHITE : figure::BLACK;
            char lower = std::tolower(c);
            std::string key = std::string(1, lower) + (team == figure::WHITE ? "w" : "b"); // FIXED была точка с запятой посередине строки
            std::unique_ptr<figure> fig;
            switch(lower){
                case 'p': fig = std::make_unique<pawn>(  team, std::pair{x,y}, textures[key]); break;
                case 'n': fig = std::make_unique<knight>(team, std::pair{x,y}, textures[key]); break;
                case 'b': fig = std::make_unique<bishop>(team, std::pair{x,y}, textures[key]); break;
                case 'r': fig = std::make_unique<rook>(  team, std::pair{x,y}, textures[key]); break;
                case 'q': fig = std::make_unique<queen>( team, std::pair{x,y}, textures[key]); break;
                case 'k': fig = std::make_unique<king>(  team, std::pair{x,y}, textures[key]); break;
                default:  break;
            }
            if(fig) setFigure(x, y, std::move(fig));
            ++x;
        }
    }

    bool whiteToMove = (side == "w"); //узнаем чья очередь ходить
    int fm = std::stoi(fmove);
    moveCount = whiteToMove ? (fm - 1)*2 : (fm - 1)*2 + 1;
        
    whiteCanCastleKingSide = (castling.find('K') != std::string::npos); //раздаем права на рокировку
    whiteCanCastleQueenSide = (castling.find('Q') != std::string::npos);
    blackCanCastleKingSide = (castling.find('k') != std::string::npos);
    blackCanCastleQueenSide = (castling.find('q') != std::string::npos);
    
    //узнаем про взятие на проходе
    if (ep != "-") {
        enPassantFlag = true;
        enPassantPosition = { ep[0] - 'a', ep[1] - '1' };
    } else {
        enPassantFlag = false;
        enPassantPosition = {-1,-1};
    }

    updateFen();
    return true;
}

bool Board::startRep(const std::string& filename, std::map<std::string, sf::Texture>& textures)
{
    std::filesystem::path saveDir = std::filesystem::current_path().parent_path() / "puzzles";
    std::filesystem::path fullpath = saveDir / filename;
    std::ifstream in(fullpath);
    if (!in.is_open()) return false;

    fens.clear(); //записываем в вектор тут
    std::string line;
    while (std::getline(in, line)) {
        if (!line.empty())
            fens.push_back(line);
    }
    if (fens.size() < 2) return false;

    index = 0; //инициализируем
    repTextures = &textures;
    return loadPosFromFEN(fens[0], textures);
}

int Board::processWhiteMove()//Она проверяет позицию щас и ту, которая в файле. Если все сходится, то он  сразу делает ход черными и возвращает ход белым
{ // return 0 — ход не сделан fen не совпал  return 1 — ход сделан успешно  return 2 — индекс вышел за границу
    if (index + 1 < fens.size()) { // если следующий fen есть
        updateFen();
        if (fenPos != fens[index + 1]) { // если не совпадает
            loadPosFromFEN(fens[index], *repTextures);  // откат
            return 0;  // несовпадение
        }
    }
    else { // следующего фена нет 
        loadPosFromFEN(fens[index], *repTextures);
        return 0;
    }

    index += 2;

    if (index >= fens.size()) { // существует ли след ход белых
        return 2;  // ходов нет - победа
    }

    loadPosFromFEN(fens[index], *repTextures);
    return 1;
}