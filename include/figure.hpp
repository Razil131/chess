#pragma once
#include <utility>
#include <vector>
#include <string>

class figure {
public:
    enum teams { BLACK, WHITE };
    enum figureTypes { BISHOP, KING, KNIGHT, PAWN, QUEEN, ROOK };

    

protected:
    teams team;
    std::pair<int, int> pos;
    figureTypes figureType;

public:
    std::string iconPath; // хз как правильно эти public расположить чтобы их не два было
    virtual ~figure() = default;

    virtual teams getTeam();
    virtual void setTeam(const teams new_team);
    virtual std::pair<int, int> getPos();
    virtual void setPos(std::pair<int, int> new_pos);
    virtual figureTypes getFigureType();
    virtual void setFigureType(const figureTypes new_figureType);
    virtual std::string getIconPath();

    virtual std::vector<std::pair<int, int>> get_available_moves() = 0;
};
