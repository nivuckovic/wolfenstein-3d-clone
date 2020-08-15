#include "State.h"

State::State(sf::RenderWindow& window) : m_context(window) {
}

Context& State::getContext() {
	return m_context;
}

sf::RenderWindow& State::getWindow() {
	return getContext().m_window;
}
