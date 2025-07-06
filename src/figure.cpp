#include "figure.hpp"

figure::teams figure::getTeam() {
        return team;
}

void figure::setTeam(const figure::teams new_team) {
    team = new_team;
}

std::pair<int, int> figure::getPos() {
    return pos;
}

 void figure::setPos(std::pair<int, int> new_pos) {
    pos = new_pos;
}

figure::figureTypes figure::getFigureType() {
    return figureType;
}

std::string figure::getIconPath() {
    return iconPath;
}


void figure::setFigureType(const figure::figureTypes new_figureType) {
    figureType = new_figureType;
}


sf::Sprite* figure::getSprite(){
    return &sprite;
}

void figure::setSprite(const sf::Sprite new_sprite){
    sprite = new_sprite;
}

