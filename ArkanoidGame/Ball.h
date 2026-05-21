#pragma once
#include "SFML/Graphics.hpp"
#include "GameObject.h"

namespace SnakeGame
{
	class Ball : public GameObject
	{
	public:
		void Init() override;
		void Update(float timeDelta) override;
		void Draw(sf::RenderWindow& window) override;

		void ReboundFromPlatform();
		void ReboundFromBlock();		
		void ReboundFromBlockSide();

	private:
		sf::Vector2f direction;
	};
}