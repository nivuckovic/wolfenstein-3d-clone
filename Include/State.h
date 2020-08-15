#pragma once

#include "SFML/Graphics.hpp"
#include "Context.h"

class State {
public:
	State(sf::RenderWindow& window);

	virtual bool input(sf::Event& event) = 0;
	virtual bool update(sf::Time& dt) = 0;
	virtual bool render() = 0;

	Context& getContext();
	sf::RenderWindow& getWindow();

private:
	Context m_context;
};

