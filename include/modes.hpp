#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "figure.hpp"
#include "textures.hpp"
#include "board.hpp"
#include "ui_board.hpp"
#include "events.hpp"
#include "ui_figures.hpp"
#include "end_game.hpp"
#include "castling.hpp"
#include "engine.hpp"


void vsPlayerStandart(sf::RenderWindow& window,sf::Font& font);

void vsPlayerFisher(sf::RenderWindow& window,sf::Font& font);

void vsComputerStandart(sf::RenderWindow& window,sf::Font& font, figure::teams userTeam);

void vsComputerFisher(sf::RenderWindow& window,sf::Font& font, figure::teams userTeam);

void vsPlayer3Check(sf::RenderWindow& window,sf::Font& font);

void vsComputer3Check(sf::RenderWindow& window,sf::Font& font, figure::teams userTeam);