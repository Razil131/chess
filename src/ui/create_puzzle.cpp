#include "create_puzzle.hpp"

void createChooseFigureMenuCreatePuzzle(sf::RenderWindow& window,
    std::map<std::string, sf::Texture>& textures,
    std::pair<int,int> selectedCell,
    int cellSize, int offsetX, int offsetY,
    sf::RectangleShape& container,
    std::vector<sf::Sprite>& sprites,
    std::vector<sf::RectangleShape>& backgroundShapes)
{
    sprites.clear();
    backgroundShapes.clear();

    std::vector<std::string> types = {"k","q","b","n","r","p"}; // все фигуры
    const int cols = (int)types.size(); // колонки
    const int rows = 2; // строки
    const float padding = 5.f; // отступ
    const float scaleFactor = 1.f / 2.f; // размер иконок на панельке этой 

    float iconSize = cellSize * scaleFactor;
    float stepX = iconSize + padding;
    float stepY = iconSize + padding;

    float menuW = cols * stepX + padding;
    float menuH = rows * stepY + padding;

    float baseX = offsetX + selectedCell.first * cellSize;
    float baseY = offsetY + (7-selectedCell.second) * cellSize;

    float menuX = baseX - menuW / 2;
    float menuY = baseY - menuH - padding;

    if (menuX < 0) menuX = padding;
    if (menuX + menuW > window.getSize().x) menuX = window.getSize().x - menuW - padding;
    if (menuY < 0) menuY = baseY + padding;
    if (menuY + menuH > window.getSize().y) menuY = window.getSize().y - menuH - padding;

    container.setSize({ menuW, menuH }); // настраиваем контейнер в котором все кнопки
    container.setPosition(menuX, menuY);
    container.setFillColor(sf::Color(50, 50, 50, 200));
    container.setOutlineColor(sf::Color::Black);
    container.setOutlineThickness(2.f);

    for (int row = 0; row < rows; ++row) {    // создаем фон и спрайт для каждой иконки
        for (int col = 0; col < cols; ++col) {
            float x = menuX + padding + col * stepX;
            float y = menuY + padding + row * stepY;

            sf::RectangleShape bg({ iconSize, iconSize });  // фон иконки
            bg.setPosition(x, y);
            bg.setFillColor(sf::Color(200, 200, 200, 150));
            bg.setOutlineColor(sf::Color::Black);
            bg.setOutlineThickness(1.f);
            backgroundShapes.push_back(bg);

            std::string key = types[col] + std::string(row == 0 ? "w" : "b");
            sf::Sprite sp; // спрайт
            sp.setTexture(textures.at(key));
            float innerOffset = 2.f;// масштаб под размер iconSize (с учётом отступов внутри)
            float scale = (iconSize - innerOffset * 2) / textures.at(key).getSize().y;
            sp.setScale(scale, scale);
            sf::FloatRect bnds = sp.getGlobalBounds();
            sp.setPosition(
                x + (iconSize - bnds.width) / 2,
                y + (iconSize - bnds.height) / 2
            );
            sprites.push_back(sp);
        }
    }
}

void drawChooseFigureMenuCreatePuzzle(sf::RenderWindow& window,
    sf::RectangleShape& container,
    std::vector<sf::Sprite>& sprites,
    std::vector<sf::RectangleShape>& backgrounds)
{
    window.draw(container);
    for (auto& bg : backgrounds) window.draw(bg);
    for (auto& sp : sprites) window.draw(sp);
}

void drawNextModeButton(sf::RenderWindow& window, sf::RectangleShape& nextModeBtnShape, sf::Font& font, int cellSize, int offsetX, int offsetY){
    nextModeBtnShape = makeButton(25,700,sf::Color(0,255,0),sf::Color(0,0,0));
    nextModeBtnShape.setPosition(offsetX*1.5+cellSize*8,offsetY+cellSize*4);
    window.draw(nextModeBtnShape);
    drawLabel(window,font,nextModeBtnShape,">",24);
}