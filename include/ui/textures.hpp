#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <string>


// загрузить текстуры
void loadTextures(std::map<std::string, sf::Texture>& textures);



bool loadFont(sf::Font& font); // загрузить шрифт
