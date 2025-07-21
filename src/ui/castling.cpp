#include "castling.hpp"
#include "menu.hpp"

void drawCastleButtons(sf::RenderWindow& window,sf::RectangleShape& rightCastle, sf::RectangleShape& leftCastle,sf::Font& font){// отрисовать кнопки рокировки для фишера
    rightCastle = makeButton(50, 50, sf::Color(177, 199, 90), sf::Color::Black); // создаем кнопку
    rightCastle.setPosition(875, window.getSize().y/2+25); // ставим на позицию
    window.draw(rightCastle); // отрисовываем
    drawLabel(window, font, rightCastle, "right\ncastle", 18); // и отрисовываем текст на ней

    leftCastle = makeButton(50, 50, sf::Color(177, 199, 90), sf::Color::Black); // создаем кнопку
    leftCastle.setPosition(875, window.getSize().y/2-25); // ставим на позицию
    window.draw(leftCastle); // отрисовываем
    drawLabel(window, font, leftCastle, "left\ncastle", 18); // и отрисовываем текст на ней
} 