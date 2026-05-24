#pragma once
#include "SFML/Graphics.hpp"
#include "GameObject.h"
#include <algorithm>

namespace ArkanoidGame
{
	class Ball;

	class Platform : public GameObject
	{
	public:
		void Init();
		void Update(float timeDelta);
		void Draw(sf::RenderWindow& window) override;

		bool CheckCollisionWithBall(const Ball& ball);

		sf::FloatRect GetRect() const { return sprite.getGlobalBounds(); }		

	private:
		void Move(float speed);
	};
}