#pragma once

#include "SFML/Graphics.hpp"

struct Context {
	Context(sf::RenderWindow& window) : m_window(window) {};

	sf::RenderWindow& m_window;
};