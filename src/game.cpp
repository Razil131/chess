#include "../include/game.hpp"


void addFigure( // функция для добавления фигур
    std::vector<ChessFigure>& figures,
    std::map<std::string, sf::Texture>& textures,
    const std::string& key, // ключ чтобы хот как то их различать
    int x,
    int y,
    float offsetX,
    float offsetY,
    float cellSize
) {
    ChessFigure figure;
    figure.sprite.setTexture(textures[key]); // ставим текстурку
    figure.sprite.setScale(1.2f, 1.2f); // увеличиваем чуть чуть иконку а то они маленькие
    figure.sprite.setPosition(offsetX + 10 + x * cellSize, offsetY + 15 + y * cellSize); // ставим позицию x-столбец y-строка
    figure.type = key;
    figures.push_back(figure);
}

void setupFigures(
    std::map<std::string, sf::Texture>& textures,
    std::vector<ChessFigure>& figures,
    float offsetX,
    float offsetY,
    float cellSize)
{
    // список названий
    const std::string types[] = { "p", "r", "n", "b", "q", "k" };
    const std::string colors[] = { "w", "b" };  // w - white, b - black

    // загружаем все текстуры
    for (const std::string& color : colors) {
        for (const std::string& type : types) {
            std::string key = type + color;
            std::string filename = "../materials/" + key + ".png";
            textures[key] = sf::Texture(); // текстура из sfml
            if (!textures[key].loadFromFile(filename)) { // загружаем текстура если не загрузилась то ошибка :(
                throw std::runtime_error("Failed to load " + filename);
            }
        }
    }

    // добавляем белые фигуры
    for (int i = 0; i < 8; i++)
        addFigure(figures, textures, "pw", i, 6, offsetX, offsetY, cellSize);

    addFigure(figures, textures, "rw", 0, 7, offsetX, offsetY, cellSize);
    addFigure(figures, textures, "nw", 1, 7, offsetX, offsetY, cellSize);
    addFigure(figures, textures, "bw", 2, 7, offsetX, offsetY, cellSize);
    addFigure(figures, textures, "qw", 3, 7, offsetX, offsetY, cellSize);
    addFigure(figures, textures, "kw", 4, 7, offsetX, offsetY, cellSize);
    addFigure(figures, textures, "bw", 5, 7, offsetX, offsetY, cellSize);
    addFigure(figures, textures, "nw", 6, 7, offsetX, offsetY, cellSize);
    addFigure(figures, textures, "rw", 7, 7, offsetX, offsetY, cellSize);

    // Добавляем черные фигуры
    for (int i = 0; i < 8; i++)
        addFigure(figures, textures, "pb", i, 1, offsetX, offsetY, cellSize);

    addFigure(figures, textures, "rb", 0, 0, offsetX, offsetY, cellSize);
    addFigure(figures, textures, "nb", 1, 0, offsetX, offsetY, cellSize);
    addFigure(figures, textures, "bb", 2, 0, offsetX, offsetY, cellSize);
    addFigure(figures, textures, "qb", 3, 0, offsetX, offsetY, cellSize);
    addFigure(figures, textures, "kb", 4, 0, offsetX, offsetY, cellSize);
    addFigure(figures, textures, "bb", 5, 0, offsetX, offsetY, cellSize);
    addFigure(figures, textures, "nb", 6, 0, offsetX, offsetY, cellSize);
    addFigure(figures, textures, "rb", 7, 0, offsetX, offsetY, cellSize);

}
