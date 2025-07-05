#pragma once
#include <utility>
#include <vector>
#include <string>

class Board;

class figure { //общий класс фигур
public:
    enum teams { BLACK, WHITE }; //цвет
    enum figureTypes { BISHOP, KING, KNIGHT, PAWN, QUEEN, ROOK }; //тип

    

protected: //
    teams team;
    std::pair<int, int> pos;
    figureTypes figureType;

public:// хз как правильно эти public расположить чтобы их не два было
    std::string iconPath; //путь к изображению
    virtual ~figure() = default; //деструктор

    //геттеры
    virtual teams getTeam();
    virtual void setTeam(const teams new_team);
    virtual std::pair<int, int> getPos();
    virtual void setPos(std::pair<int, int> new_pos);
    virtual figureTypes getFigureType();
    virtual void setFigureType(const figureTypes new_figureType);
    virtual std::string getIconPath();

    virtual std::vector<std::pair<int, int>> get_available_moves(const Board& board) = 0; //виртуальный метод чисто для наследования
};
