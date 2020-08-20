#pragma once
#include "State.h"

#include <array>

class PlayingState : public State {
public:
	PlayingState(sf::RenderWindow& window);

	bool input(sf::Event& event) override;
	bool update(sf::Time& dt) override;
	bool render() override;

private:
	void renderMinimap();

	void raycast();

	void drawRaycast(sf::Vector2f playerPosition);
	bool horizontalHitWall(sf::Vector2f point, bool facing);
	bool verticalHitWall(sf::Vector2f point, bool facing);

	float distance(sf::Vector2f& a, sf::Vector2f& b);

	bool checkCollision(sf::Vector2f position);
	sf::Vector2i calculateDirection(float angle);

private:
	std::vector<std::vector<int>> m_world;
	int m_worldX;
	int m_worldY;

	sf::Texture wallTexture;
	sf::Texture brickTexture;
	sf::Texture floorTexture;

	sf::Image floorImage;
	sf::Image drawImage;

	struct Player {
		sf::CircleShape m_circle;
		float m_angle;
		float m_FOV;
		float m_height;
		float m_speed;
		float m_maxViewDistance;

		float m_distanceFromScreen;
	};

	struct Hitpoint {
		bool hit;
		float textureOffset;
		float lineHeight;
		float distance;

		int texture;
	};

	Hitpoint castRay(sf::Vector2f position, float angle);

	Player m_player;
};

