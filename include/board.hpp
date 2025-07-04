#pragma once
#include <vector>
#include <memory>
#include "figure.hpp"

class Board {
public:
    Board(); //конструктор
    ~Board() = default; //деструктор
    figure* getFigure(int x, int y) const; //получить фигуру на доске 
    bool isOccupied(int x, int y) const; //проверка на занятость клетки
    bool isOccupiedByOwnTeam(int x, int y, figure::teams team) const; //проверка на то, своей ли фигурой занята клетка
    bool isOccupiedByEnemyTeam(int x, int y, figure::teams team) const; //Проверка на то, чужой ли фигурой занята клетка
    void setFigure(int x, int y, std::unique_ptr<figure> fig); //устанавливает фигуру в нужное место
    void removeFigure(int x, int y);//удаляет фигуру

private:
    std::vector<std::vector<std::unique_ptr<figure>>> board; //доска
};

/*
TODO

сделать функцию расстановки фигур
сделать функцию хода
*/