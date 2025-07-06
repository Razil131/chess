#include "game.hpp"

namespace { // это штука чтобы только из этого файла можно было функцию применить
    void addFigure( // добавить фигуру на gui
        std::unique_ptr<figure> fig, // указатель на фигуру
        std::vector<std::unique_ptr<figure>>& figures, // вектор всех фигур чтобы туда новую добавить
        float offsetX, // отступ по x для букв
        float offsetY, // отстпу по y для цифр
        float cellSize, // размер ячейки поля
        int cellX, // по x на какой ячейке должна быть фигура
        int cellY) // по y на какой ячейке должна быть фигура
    {
        fig->getSprite()->setScale(1.2f, 1.2f); // чуть увеличиваем размер иконки 
        fig->getSprite()->setPosition(
            offsetX + 10 + cellX * cellSize, // ставим на позицию +10 +15 это чтобы примерно по центру ячейки картинка была
            offsetY + 15 + cellY * cellSize
        ); 
        figures.push_back(std::move(fig)); // добавляем в вектор фигур
    }
}

void setupFigures(
    std::map<std::string, sf::Texture>& textures, // мапа название фигуры - ее текстура
    std::vector<std::unique_ptr<figure>>& figures, // вектор указателей на фигуры
    float offsetX, // отступ по x для букв
    float offsetY, // отстпу по y для цифр
    float cellSize) // размер ячейки поля
{
    const std::string types[] = { "p", "r", "n", "b", "q", "k" }; // это чтобы генерить название png в materials (36 строка генерит)
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

    // белые пешки
    for (int i = 0; i < 8; i++) {
        auto pawn_w = std::make_unique<pawn>(
            figure::WHITE, std::make_pair(i,1), textures["pw"]);
        addFigure(std::move(pawn_w), figures, offsetX, offsetY, cellSize, i, 6);
    }

    // белые основные фигуры
    addFigure(std::make_unique<rook>(
                  figure::WHITE, std::make_pair(0,0), textures["rw"]),
              figures, offsetX, offsetY, cellSize, 0, 7);
    addFigure(std::make_unique<knight>(
                  figure::WHITE, std::make_pair(1,0), textures["nw"]),
              figures, offsetX, offsetY, cellSize, 1, 7);
    addFigure(std::make_unique<bishop>(
                  figure::WHITE, std::make_pair(2,0), textures["bw"]),
              figures, offsetX, offsetY, cellSize, 2, 7);
    addFigure(std::make_unique<queen>(
                  figure::WHITE, std::make_pair(3,0), textures["qw"]),
              figures, offsetX, offsetY, cellSize, 3, 7);
    addFigure(std::make_unique<king>(
                  figure::WHITE, std::make_pair(4,0), textures["kw"]),
              figures, offsetX, offsetY, cellSize, 4, 7);
    addFigure(std::make_unique<bishop>(
                  figure::WHITE, std::make_pair(5,0), textures["bw"]),
              figures, offsetX, offsetY, cellSize, 5, 7);
    addFigure(std::make_unique<knight>(
                  figure::WHITE, std::make_pair(6,0), textures["nw"]),
              figures, offsetX, offsetY, cellSize, 6, 7);
    addFigure(std::make_unique<rook>(
                  figure::WHITE, std::make_pair(7,0), textures["rw"]),
              figures, offsetX, offsetY, cellSize, 7, 7);

    // черные пешки
    for (int i = 0; i < 8; i++) {
        auto pawn_b = std::make_unique<pawn>(
            figure::BLACK, std::make_pair(i,6), textures["pb"]);
        addFigure(std::move(pawn_b), figures, offsetX, offsetY, cellSize, i, 1);
    }

    // черные основные фигуры
    addFigure(std::make_unique<rook>(
                  figure::BLACK, std::make_pair(0,7), textures["rb"]),
              figures, offsetX, offsetY, cellSize, 0, 0);
    addFigure(std::make_unique<knight>(
                  figure::BLACK, std::make_pair(1,7), textures["nb"]),
              figures, offsetX, offsetY, cellSize, 1, 0);
    addFigure(std::make_unique<bishop>(
                  figure::BLACK, std::make_pair(2,7), textures["bb"]),
              figures, offsetX, offsetY, cellSize, 2, 0);
    addFigure(std::make_unique<queen>(
                  figure::BLACK, std::make_pair(3,7), textures["qb"]),
              figures, offsetX, offsetY, cellSize, 3, 0);
    addFigure(std::make_unique<king>(
                  figure::BLACK, std::make_pair(4,7), textures["kb"]),
              figures, offsetX, offsetY, cellSize, 4, 0);
    addFigure(std::make_unique<bishop>(
                  figure::BLACK, std::make_pair(5,7), textures["bb"]),
              figures, offsetX, offsetY, cellSize, 5, 0);
    addFigure(std::make_unique<knight>(
                  figure::BLACK, std::make_pair(6,7), textures["nb"]),
              figures, offsetX, offsetY, cellSize, 6, 0);
    addFigure(std::make_unique<rook>(
                  figure::BLACK, std::make_pair(7,7), textures["rb"]),
              figures, offsetX, offsetY, cellSize, 7, 0);
}
