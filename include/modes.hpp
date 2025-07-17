#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include "figure.hpp"
#include "create_puzzle.hpp"
#include "textures.hpp"
#include "board.hpp"
#include "ui_board.hpp"
#include "events.hpp"
#include "ui_figures.hpp"
#include "end_game.hpp"
#include "castling.hpp"
#include "engine.hpp"


void vsPlayerStandart(sf::RenderWindow& window,sf::Font& font, std::string savefile = "");

void vsPlayerFisher(sf::RenderWindow& window,sf::Font& font, std::string savefile = "");

void vsComputerStandart(sf::RenderWindow& window,sf::Font& font, figure::teams userTeam = figure::WHITE, std::string savefile = "");

void vsComputerFisher(sf::RenderWindow& window,sf::Font& font, figure::teams userTeam = figure::WHITE, std::string savefile = "");

void vsPlayer3Check(sf::RenderWindow& window,sf::Font& font, std::string savefile = "");

void vsComputer3Check(sf::RenderWindow& window,sf::Font& font, figure::teams userTeam = figure::WHITE, std::string savefile = "");

void createPuzzle(sf::RenderWindow& window,sf::Font& font);