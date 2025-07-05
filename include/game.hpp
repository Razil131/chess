#pragma once
#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>

// структура для хранения фигуры
struct ChessFigure {
    sf::Sprite sprite;
    std::string type; // например "pw"
};

void setupFigures(std::map<std::string, sf::Texture>& textures, std::vector<ChessFigure>& pieces, float offsetX, float offsetY, float cellSize);

void addFigure(
    std::vector<ChessFigure>& pieces,
    std::map<std::string, sf::Texture>& textures,
    const std::string& key,
    int x,
    int y,
    float offsetX,
    float offsetY,
    float cellSize
);
