#pragma once
#include "SFML/Graphics.hpp"

namespace SnakeGame
{
	class Ball
	{
	public:
		void Init();
		void Update(float timeDelta);
		void Draw(sf::RenderWindow& window);

		void ReboundFromPlatform();

		const sf::Vector2f& GetPosition() const { return sprite.getPosition(); }

		Ball() = default;
		Ball(const Ball&) = delete;
		Ball& operator=(const Ball&) = delete;
		Ball(Ball&&) = default;
		Ball& operator=(Ball&&) = default;

	private:
		sf::Sprite sprite;
		sf::Texture texture;
		sf::Vector2f direction;
		float angle = 45;
	};
}