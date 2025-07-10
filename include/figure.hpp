#pragma once
#include <utility>
#include <vector>
#include <SFML/Graphics.hpp>

class Board;

class figure { //общий класс фигур
public:
    enum teams { BLACK, WHITE }; //цвет
    enum figureTypes { BISHOP, KING, KNIGHT, PAWN, QUEEN, ROOK }; //тип

protected: 
    teams team;
    std::pair<int, int> pos;
    figureTypes figureType;
    std::string iconPath; //путь к изображению
    sf::Sprite sprite; // это типо изображение на экране на эту штуку можно поставить любую текстуру
    bool hasMoved = false;

public:// хз как правильно эти public расположить чтобы их не два было
    virtual ~figure() = default; //деструктор

    virtual std::vector<std::pair<int, int>> get_available_moves(const Board& board) = 0; //виртуальный метод чисто для наследования
    
    //геттеры и сеттеры
    virtual teams getTeam();
    virtual void setTeam(const teams new_team);
    virtual std::pair<int, int> getPos();
    virtual void setPos(std::pair<int, int> new_pos);
    virtual figureTypes getFigureType();
    virtual void setFigureType(const figureTypes new_figureType);
    virtual sf::Sprite* getSprite();
    virtual void setSprite(const sf::Sprite new_sprite);
    virtual std::string getIconPath();
    bool getMoved() const;
    void setMoved(bool m);

};
