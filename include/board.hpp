#pragma once
#include <vector>
#include <memory>
#include "figure.hpp"
#include "pawn.hpp"
#include "rook.hpp"
#include "knight.hpp"
#include "bishop.hpp"
#include "queen.hpp"
#include "king.hpp"

class Board {
public:
    Board(); //конструктор
    ~Board() = default; //деструктор
    figure* getFigure(int x, int y) const; //получить фигуру на доске 
    figure* getFigure(std::pair<int,int> pos) const; // надоело мне уже это first second
    std::vector<std::vector<std::unique_ptr<figure>>>* getBoard(); // получить всю доску
    figure::teams getCurrentTeam() const;
    bool isOccupied(int x, int y) const; //проверка на занятость клетки
    bool isOccupiedByOwnTeam(int x, int y, figure::teams team) const; //проверка на то, своей ли фигурой занята клетка
    bool isOccupiedByEnemyTeam(int x, int y, figure::teams team) const; //Проверка на то, чужой ли фигурой занята клетка
    void setFigure(int x, int y, std::unique_ptr<figure> fig); //устанавливает фигуру в нужное место
    void removeFigure(int x, int y);//удаляет фигуру
    bool makeMove(std::pair<int, int> from, std::pair<int, int> to); //Ход
    void initialize(std::map<std::string, sf::Texture>&); //функция для дефолтной расстановки фигур
    void convertPawn(int x, int y, figure::figureTypes new_type, sf::Texture& texture); //функция превращения пешки в фигуру
    bool isKingInCheck(figure::teams team) const; //проверка на шах
    std::vector<std::pair<int, int>> getValidMoves(int x, int y); //для корректной проверки на то, откроем ли мы короля под шах
    bool wouldKingInCheck(std::pair<int, int> from, std::pair<int, int> to); //для красивой жизни
    bool isKingInMate(figure::teams team); //проверка на мат
    bool isKingInStalemate(figure::teams team); //проверка на пат
    bool isSquareAttack(std::pair<int, int> square, figure::teams team) const; //проверка квадрата на атаку для рокировки
    void fisherPos(std::map<std::string, sf::Texture>& textures); //расстановка по фишеру
    bool fisherCastle(bool kingSide); //функция для рокировки фишера
    void updateFen(); //функция для обновления фен позиции
    bool exportToFile(const std::string& filename, int players, int mode); // функция для создания сохранения
    bool importFromFile(const std::string& filename,  std::map<std::string, sf::Texture>& textures); //для загрузки
    void clear(); // отчистить доску от фигур



    bool convertFlag = false; //флаг для convertPawn
    std::pair<int, int> convertPosition; //позиция пешки, которую надо превратить
    bool enPassantFlag = false; //флаг для взятия на проходе
    std::pair<int, int> enPassantPosition; //позиция пешки для взятия на проходе
    std::pair<std::pair<int, int>, std::pair<int, int>> lastMove; //переменная для хранения последнего хода
    bool mateFlag = false;//флаг для мата
    bool staleMateFlag = false; //флаг для пата
    bool castleflag = false;
    bool whiteCanCastleKingSide;//права на рокировку
    bool whiteCanCastleQueenSide;
    bool blackCanCastleKingSide;
    bool blackCanCastleQueenSide;
    int whiteRookKS; //координаты ладей
    int whiteRookQS;
    int blackRookKS;
    int blackRookQS;

    std::vector<std::string> movesUCI; //будем тут хранить ходы в формате для движка
    std::string fenPos; //тут мы храним позицию для фишера вс компьютера


private:
    std::vector<std::vector<std::unique_ptr<figure>>> board; //доска
    int moveCount;

};