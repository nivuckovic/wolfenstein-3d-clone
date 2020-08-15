#include "PlayingState.h"

#include <iostream>
#include <vector>

PlayingState::PlayingState(sf::RenderWindow& window) : State(window) {
	m_world = { 
		{
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 2, 2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 2, 0, 0, 0, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 2, 2, 0, 2, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		} 
	};

	m_worldX = m_world.size();
	m_worldY = m_world[0].size();

	m_player.m_circle = sf::CircleShape(10.f);
	m_player.m_circle.setPosition(sf::Vector2f(156.f, 156.f));
	m_player.m_circle.setFillColor(sf::Color::Blue);
	m_player.m_circle.setOrigin(sf::Vector2f(10.f, 10.f));
	
	m_player.m_angle = 0.0f;
	m_player.m_FOV = 3.14f / 3;
	m_player.m_speed = 200.f;
	m_player.m_maxViewDistance = 640.f;

	m_player.m_distanceFromScreen = 320 / std::tan(m_player.m_FOV);

	if (!wallTexture.loadFromFile("wall_texture.png")) {
		std::cout << "Ne mogu ucitat wall_texture.png!" << std::endl;
	}
	if (!brickTexture.loadFromFile("brick_texture.png")) {
		std::cout << "Ne mogu ucitat brick_texture.png!" << std::endl;
	}

	brickTexture.setRepeated(true);
	wallTexture.setRepeated(true);
}

bool PlayingState::input(sf::Event& event) {

	return false;
}

bool PlayingState::update(sf::Time& dt) {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		sf::Vector2f oldPosition = m_player.m_circle.getPosition();

		m_player.m_circle.move(m_player.m_speed * dt.asSeconds() * std::cos(m_player.m_angle + 3.14f / 6), m_player.m_speed * dt.asSeconds() * std::sin(m_player.m_angle + 3.14f / 6));
	
		if (checkCollision(m_player.m_circle.getPosition())) {
			m_player.m_circle.setPosition(oldPosition);
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		m_player.m_circle.move(m_player.m_speed * dt.asSeconds() * -std::cos(m_player.m_angle + 3.14f / 6), m_player.m_speed * dt.asSeconds() * -std::sin(m_player.m_angle + 3.14f / 6));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
	
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
	
	}

	int mousePositionX = sf::Mouse::getPosition(getWindow()).x;
	float rotationSpeed = 0.045f;

	if (mousePositionX < 100) {
		m_player.m_angle -= rotationSpeed;
		
		if (m_player.m_angle <= 0)
			m_player.m_angle += 2.f * 3.14f;
	}

	if (mousePositionX > 540) {
		m_player.m_angle += rotationSpeed;


		if (m_player.m_angle >= 2.f * 3.14f)
			m_player.m_angle -= 2.f * 3.14f;
	}


	return false;
}

bool PlayingState::render() {
	sf::View defaultView(sf::FloatRect(0, 0, 640.f, 640.f));
	getWindow().setView(defaultView);

	raycast();
	renderMinimap();

	for (std::size_t i = 1; i < 10; ++i) {
		sf::Vertex line[] = {
			sf::Vertex(sf::Vector2f(i * 64, 0), sf::Color::Blue),
			sf::Vertex(sf::Vector2f(i * 64, 640), sf::Color::Blue)
		};

		getWindow().draw(line, 2, sf::Lines);
		
		sf::Vertex line2[]{
			sf::Vertex(sf::Vector2f(0, i * 64), sf::Color::Blue),
			sf::Vertex(sf::Vector2f(640, i * 64), sf::Color::Blue)
		};

		getWindow().draw(line2, 2, sf::Lines);
	}

	return false;
}

void PlayingState::renderMinimap() {
	sf::View minimapView;
	minimapView.setViewport(sf::FloatRect(0.8f, 0.05f, 0.25f, 0.25f));
	getWindow().setView(minimapView);

	
	sf::VertexArray block(sf::Quads, 4);

	sf::Vector2i playerCoords_int(std::floor(m_player.m_circle.getPosition().x / 64.f), std::floor(m_player.m_circle.getPosition().y / 64.f));

	int leftX = std::max(0, playerCoords_int.y - 5);
	int rightX = std::min(m_worldX, playerCoords_int.y + (10 - (playerCoords_int.y - leftX)));

	leftX -= 10 - (rightX - leftX);

	int leftY = std::max(0, playerCoords_int.x - 5);
	int rightY = std::min(m_worldY, playerCoords_int.x + (10 - (playerCoords_int.x - leftY)));

	leftY -= 10 - (rightY - leftY);

	sf::Vector2f oldPosition = m_player.m_circle.getPosition();
	m_player.m_circle.setPosition(oldPosition - sf::Vector2f(leftY * 64.f, leftX * 64.f));
	getWindow().draw(m_player.m_circle);
	drawRaycast(m_player.m_circle.getPosition());
	m_player.m_circle.setPosition(oldPosition);

	for (std::size_t i = leftX; i < rightX; ++i) {
		for (std::size_t j = leftY; j < rightY; ++j) {
			if (m_world[i][j] == 0)
				continue;

			block[0].position = sf::Vector2f((j - leftY) * 64, (i - leftX) * 64);
			block[1].position = sf::Vector2f(((j - leftY) + 1) * 64, (i - leftX) * 64);
			block[2].position = sf::Vector2f(((j - leftY) + 1) * 64, ((i - leftX) + 1) * 64);
			block[3].position = sf::Vector2f((j - leftY) * 64, ((i - leftX) + 1) * 64);
			
			block[0].color = sf::Color::Red;
			block[1].color = sf::Color::Red;
			block[2].color = sf::Color::Red;
			block[3].color = sf::Color::Red;

			getWindow().draw(block);
		}
	}

}

void PlayingState::raycast() {	
	sf::Vector2f playerPosition = m_player.m_circle.getPosition();

	float anglePerLine = m_player.m_FOV / 640;

	std::vector<sf::Vector2f> intersections;

	for (size_t i = 0; i < 640; ++i) {
		float rayAngle = std::fmodf(m_player.m_angle + anglePerLine * i, 2.f * 3.14f);

		Hitpoint hitpoint = castRay(m_player.m_circle.getPosition(), rayAngle);

		if (!hitpoint.hit) {
			continue;
		}
			
		sf::VertexArray line(sf::Lines, 2);

		line[0].position = sf::Vector2f(i, 320.f - hitpoint.lineHeight);
		line[1].position = sf::Vector2f(i, 320.f + hitpoint.lineHeight);

		line[0].texCoords = sf::Vector2f(hitpoint.textureOffset, 0.f);
		line[1].texCoords = sf::Vector2f(hitpoint.textureOffset, 256.f);

		line[0].color = sf::Color(255, 255, 255, 255 - hitpoint.distance / m_player.m_maxViewDistance * 255);
		line[1].color = sf::Color(255, 255, 255, 255 - hitpoint.distance / m_player.m_maxViewDistance * 255);

		sf::RenderStates state;

		state.texture = &wallTexture;
		/*
		switch (hitpoint.texture) {
		case 1: 
			state.texture = &wallTexture;
			break;
		case 2:
			state.texture = &brickTexture;
			break;
		}
		*/

		getWindow().draw(line, state);
	}
}

void PlayingState::drawRaycast(sf::Vector2f playerPosition) {

	float lineLength = 640.f;
	float anglePerLine = m_player.m_FOV / 640;

	for (size_t i = 0; i < 640; ++i) {
		if (i != 0 && i != 639)
			continue;

		sf::Vertex line[] = {
		sf::Vertex(playerPosition),
		sf::Vertex(playerPosition + sf::Vector2f(lineLength * std::cos(m_player.m_angle + anglePerLine * i), lineLength * std::sin(m_player.m_angle + anglePerLine * i)))
		};

		getWindow().draw(line, 2, sf::Lines);
	}

}

bool PlayingState::horizontalHitWall(sf::Vector2f point, bool facing) {
	sf::Vector2i point_int(std::floor(point.x / 64.f), std::floor(point.y / 64.f) - ((facing) ? 0 : 1));

	if (point_int.x >= 0 && point_int.x < m_worldY && point_int.y >= 0 && point_int.y < m_worldX) {
		if (m_world[point_int.y][point_int.x]) {
			return true;
		}
	}

	return false;
}

bool PlayingState::verticalHitWall(sf::Vector2f point, bool facing) {
	sf::Vector2i point_int(std::floor(point.x / 64.f) - ((facing) ? 0 : 1), std::floor(point.y / 64.f));

	if (point_int.x >= 0 && point_int.x < m_worldY && point_int.y >= 0 && point_int.y < m_worldX) {
		if (m_world[point_int.y][point_int.x]) {
			return true;
		}
	}

	return false;
}

float PlayingState::distance(sf::Vector2f& a, sf::Vector2f& b) {
	float offset_X = b.x - a.x;
	float offset_Y = b.y - a.y;
	
	return std::sqrt(std::pow(offset_X, 2) + std::pow(offset_Y, 2));
}

bool PlayingState::checkCollision(sf::Vector2f position) {
	int posX = std::floor(position.x / 64.f);
	int posY = std::floor(position.y / 64.f);

	if (m_world[posY][posX])
		return true;

	return false;
}

PlayingState::Hitpoint PlayingState::castRay(sf::Vector2f position, float angle) {
	
	bool horizontal_hit = false;
	bool vertical_hit = false;

	sf::Vector2f horizontal_point;
	sf::Vector2f vertical_point;

	float distanceLeft = m_player.m_maxViewDistance;

	// HORIZONTAL
	sf::Vector2f A_coords;
	bool horizontalFacingUp = (angle < 3.14f) ? true : false;

	float offsetY_A = horizontalFacingUp ? 64.f - std::fmodf(position.y, 64.f) : -std::fmodf(position.y, 64.f);
	float distance_A = offsetY_A / std::sin(angle);

	distanceLeft -= distance_A;

	if (distanceLeft >= 0) {
		A_coords = sf::Vector2f(position.x + distance_A * std::cos(angle), std::round(position.y + distance_A * std::sin(angle)));

		if (horizontalHitWall(A_coords, horizontalFacingUp)) {
			horizontal_hit = true;
			horizontal_point = A_coords;
		}
		else {
			float distance = std::fabsf(64.f / std::sin(angle) * (horizontalFacingUp ? 1 : -1));

			// Ako vidis dok nije pogodeno provjeravaj
			for (std::size_t j = 1; (distanceLeft - distance) >= 0; ++j) {
				sf::Vector2f point = sf::Vector2f(A_coords.x + j * distance * std::cos(angle), std::round(A_coords.y + j * distance * std::sin(angle)));

				if (horizontalHitWall(point, horizontalFacingUp)) {
					horizontal_hit = true;
					horizontal_point = point;

					break;
				}

				distanceLeft -= distance;
			}
		}
	}

	// VERTICAL
	sf::Vector2f B_coords;
	bool verticalFacingUp = (angle > 3.14f * 3.f / 2.f || angle < 3.14f / 2.f) ? true : false;

	float offsetX_B = verticalFacingUp ? 64.f - std::fmodf(position.x, 64.f) : -std::fmodf(position.x, 64.f);
	float distance_B = offsetX_B / std::cos(angle);

	distanceLeft = m_player.m_maxViewDistance;
	distanceLeft -= distance_B;

	if (distanceLeft >= 0) {
		B_coords = sf::Vector2f(std::round(position.x + distance_B * std::cos(angle)), position.y + distance_B * std::sin(angle));

		if (verticalHitWall(B_coords, verticalFacingUp)) {
			vertical_hit = true;
			vertical_point = B_coords;
		}
		else {
			float distance = std::fabsf(64.f / std::cos(angle) * (verticalFacingUp ? 1 : -1));

			// Ako vidis dok nije pogodeno provjeravaj
			for (std::size_t j = 1; (distanceLeft - distance) >= 0; ++j) {
				sf::Vector2f point = sf::Vector2f(std::round(B_coords.x + j * distance * std::cos(angle)), B_coords.y + j * distance * std::sin(angle));

				if (verticalHitWall(point, verticalFacingUp)) {
					vertical_hit = true;
					vertical_point = point;

					break;
				}

				distanceLeft -= distance;
			}
		}
	}

	float cast_point_distance = 0.f;
	float textureOffset = 0.f;

	sf::Vector2f point;
	
	if (vertical_hit && horizontal_hit) {
		float vertical_distance = distance(vertical_point, position);
		float horizontal_distance = distance(horizontal_point, position);

		point = (vertical_distance < horizontal_distance) ? vertical_point : horizontal_point;

		textureOffset = std::fmodf((vertical_distance < horizontal_distance) ? vertical_point.y : horizontal_point.x, 64.f);

		cast_point_distance = std::fminf(vertical_distance, horizontal_distance);
	}
	else if (vertical_hit) {
		cast_point_distance = distance(vertical_point, position);
		textureOffset = std::fmodf(vertical_point.y, 64.f);
		point = vertical_point;
	}
	else if (horizontal_hit) {
		cast_point_distance = distance(horizontal_point, position);
		textureOffset = std::fmodf(horizontal_point.x, 64.f);
		point = horizontal_point;
	}
	else {
		return Hitpoint();
	}

	float wallHeight = 128.f;
	float lineHeight = wallHeight / cast_point_distance * m_player.m_distanceFromScreen;

	Hitpoint result;
	result.hit = true;
	result.lineHeight = lineHeight;
	result.textureOffset = textureOffset;
	result.distance = cast_point_distance;
	// Popravi ovo treba promjeniti jer pogodena tocka moze biti za jedan veca od one na kojoj je tekstura oznacena
	result.texture = m_world[std::floor(point.y / 64.f)][std::floor(point.x / 64.f)];

	return result;
}
