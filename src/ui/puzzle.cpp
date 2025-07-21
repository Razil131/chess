#include "puzzle.hpp"

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
    window.draw(nextModeBtnShape);
    drawLabel(window,font,nextModeBtnShape,">",24);
}

// вернуть количество файлов в папке puzzles (задач)
int countPuzzles(const std::string& folder) { 
    int count = 0;
    if (!fs::exists(folder) || !fs::is_directory(folder))
        return 0;

    for (const auto& entry : fs::directory_iterator(folder)) {
        if (entry.is_regular_file() && entry.path().extension() == ".fen")
            ++count;
    }
    return count;
}

std::string generatePuzzleFilename(const std::string& folder) {
    std::set<int> existing;

    if (fs::exists(folder) && fs::is_directory(folder)) {  // записываем в set все номера пазлов которые есть
        for (auto& entry : fs::directory_iterator(folder)) {
            if (!entry.is_regular_file()) continue;
            auto name = entry.path().filename().string();
            // ожидаем формат "puzzle_ddd.fen"
            if (name.rfind("puzzle_", 0) != 0) continue;
            if (name.size() < 12 || name.substr(name.size()-4) != ".fen") continue;
            // извлечь цифры
            std::string num = name.substr(7, name.size() - 11); // от "puzzle_" до ".fen"
            try {
                existing.insert(std::stoi(num));
            } catch(...) {}
        }
    }

    // найдем первый не занятой номер
    int candidate = 1;
    while (existing.count(candidate)) {
        ++candidate;
    }

    std::ostringstream ss; // создаем название нового пазла
    ss << "puzzle_"
       << std::setw(3) << std::setfill('0')
       << candidate
       << ".fen";
    return ss.str();
}

// вернуть название пазла по его номеру
std::string makePuzzleFilename(std::string& numberStr) {
    int num = std::stoi(numberStr);
    
    std::ostringstream oss;
    oss << "puzzle_" << std::setw(3) << std::setfill('0') << num << ".fen";
    
    return oss.str();
}

//удалить пазл по номеру
bool deletePuzzleByNumber(const std::string& numberStr, const std::string& folder) {
    try {
        std::string nameCopy = numberStr; // так как makePuzzleFilename принимает ссылку
        std::string filename = makePuzzleFilename(nameCopy); // -> puzzle_003.fen
        std::string fullPath = folder + "/" + filename;

        if (fs::exists(fullPath)) {
            return fs::remove(fullPath); // true — если удалён
        } else {
            return false; // файла не было
        }
    } catch (const std::exception& e) {
        std::cerr << "Error deleting puzzle: " << e.what() << std::endl;
        return false;
    }
}

// функция возвращает номера всех существующих пазлов
std::vector<int> getPuzzleNumbers(const std::string& folder) {
    std::vector<int> numbers;

    if (fs::exists(folder) && fs::is_directory(folder)) { // проходимся по всем файлам записываем номера существующих
        for (const auto& entry : fs::directory_iterator(folder)) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();

                if (filename.rfind("puzzle_", 0) == 0 && filename.size() > 11 && filename.substr(filename.size() - 4) == ".fen") {
                    std::string numStr = filename.substr(7, filename.size() - 11);
                    try {
                        int num = std::stoi(numStr);
                        numbers.push_back(num);
                    } catch (...) {
                        continue; // пропускаем кривые имена
                    }
                }
            }
        }
    }

    std::sort(numbers.begin(), numbers.end()); // сортируем по порядку
    return numbers;
}
