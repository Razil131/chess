#include "textures.hpp"

void loadTextures(std::map<std::string, sf::Texture>& textures){
    const std::string types[] = {"b", "p", "r", "n", "q", "k" }; // это чтобы генерить название png в materials
    const std::string colors[] = { "w", "b" };
    // загружаем текстуры
    for (const std::string& color : colors) {
        for (const std::string& type : types) {
            std::string key = type + color;
            std::string filename = "../materials/" + key + ".png";
            textures[key] = sf::Texture(); // заполняем мапу
            if (!textures[key].loadFromFile(filename)) {
                throw std::runtime_error("Failed to load " + filename); // если нет картинки ошибка
            }
        }
    }
}




bool loadFont(sf::Font& font) {
    // загружаем шрифт для букв и цифр
    if (!font.loadFromFile("../arial.ttf")) {
        return false; // если не загрузился то :(
    }
    return true;
}