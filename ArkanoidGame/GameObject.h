#pragma once
#include <SFML/Graphics.hpp>


namespace SnakeGame
{
	class GameObject
	{
	public:
		virtual ~GameObject() = default;

		virtual void Init() = 0;
		virtual void Update(float timeDelta) = 0;
		virtual void Draw(sf::RenderWindow& window) = 0;

		const sf::Vector2f& GetPosition() const { return sprite.getPosition(); }
		const sf::FloatRect GetRect() const { return sprite.getGlobalBounds(); }
		void SetPosition(const sf::Vector2f& pos) { sprite.setPosition(pos); }

	protected:
		sf::Sprite sprite;
		sf::Texture texture;
	};
}