#pragma once
#include "SFML/Graphics.hpp"
#include <algorithm>

namespace SnakeGame
{
	class Ball;

	class Platform
	{
	public:
		void Init();
		void Update(float timeDelta);
		void Draw(sf::RenderWindow& window);
		void Move(float speed);

		bool CheckCollisionWithBall(const Ball& ball);

		sf::FloatRect GetRect() const { return sprite.getGlobalBounds(); }

		Platform() = default;
		Platform(const Platform&) = delete;
		Platform& operator=(const Platform&) = delete;
		Platform(Platform&&) = default;
		Platform& operator=(Platform&&) = default;

	private:
		sf::Sprite sprite;
		sf::Texture texture;
		float speed = 0.f;
	};
}