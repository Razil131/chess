#include "end_game.hpp"
void drawEndGameScreen(sf::RenderWindow& window,  // отрисовать экран конца игры
                       figure::teams winner,
                       const sf::Font& font,
                       sf::RectangleShape& btnRect)
{
    sf::Text winText(winner == figure::WHITE ? "WHITE WINS" : // текст
                     (winner == figure::BLACK ? "BLACK WINS" : "DRAW"),
                     font, 64);
    winText.setFillColor(sf::Color::White);
    auto winBounds = winText.getLocalBounds();
    winText.setOrigin(winBounds.width / 2.f, winBounds.height / 2.f);
    winText.setPosition(window.getSize().x / 2.f,
                        window.getSize().y / 2.f - 120.f);
    window.draw(winText);

    const sf::Vector2f winSize(window.getSize()); // кнопка
    btnRect = makeButton(300.f, 80.f,
                         sf::Color(180, 180, 180),  // фон
                         sf::Color::Black);         // обводка
    btnRect.setPosition(winSize.x / 2.f,
                        winSize.y / 2.f + 20.f);
    window.draw(btnRect);

    drawLabel(window, font, btnRect, "Return to\n   menu", 32); // текст на кнопке
}

void drawEndGameScreenPuzzle(sf::RenderWindow& window, //отрисовать меню завершения игры в пазлах
    bool win,
    const sf::Font& font,
    sf::RectangleShape& btnRect){
        sf::Text winText(win? "YOU WIN :)" : "YOU LOSE :(",font, 64);
    winText.setFillColor(sf::Color::White);
    auto winBounds = winText.getLocalBounds();
    winText.setOrigin(winBounds.width / 2.f, winBounds.height / 2.f);
    winText.setPosition(window.getSize().x / 2.f,
                        window.getSize().y / 2.f - 120.f);
    window.draw(winText);

    const sf::Vector2f winSize(window.getSize()); // кнопка
    btnRect = makeButton(300.f, 80.f,
                         sf::Color(180, 180, 180),  // фон
                         sf::Color::Black);         // обводка
    btnRect.setPosition(winSize.x / 2.f,
                        winSize.y / 2.f + 20.f);
    window.draw(btnRect);

    drawLabel(window, font, btnRect, "Return to\n   menu", 32); // текст на кнопке
    }