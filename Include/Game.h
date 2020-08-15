#pragma once

#include "SFML/Graphics.hpp"
#include "PlayingState.h"

class Game {
public:
	Game();

	void run();

private:
	void update(sf::Time& dt);
	void input();
	void render();

private:
	sf::RenderWindow m_window;
	PlayingState m_currentState;

	sf::Time m_timePerFrame;
};

