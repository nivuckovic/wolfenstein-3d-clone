#include "Game.h"
#include "..\Include\Game.h"

Game::Game() : m_window(sf::VideoMode(640, 640), "3d-raycasting-shooter"), m_timePerFrame(sf::seconds(1.f / 60.f)), m_currentState(m_window) {

}

void Game::run() {
	sf::Clock clock;
	sf::Time elapsedTime(sf::seconds(0));


	while(m_window.isOpen()) {
		elapsedTime += clock.restart();

		while (elapsedTime > m_timePerFrame) {
			elapsedTime -= m_timePerFrame;

			input();
			update(m_timePerFrame);
			render();
		}
	}
}

void Game::update(sf::Time& dt) {
	m_currentState.update(dt);
}

void Game::input() {
	sf::Event event;
	while (m_window.pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			m_window.close();
		}

		m_currentState.input(event);
	}
}

void Game::render() {
	m_window.clear();

	m_currentState.render();

	m_window.display();
}
