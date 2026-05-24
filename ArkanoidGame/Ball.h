#pragma once
#include "SFML/Graphics.hpp"
#include "GameObject.h"
#include "Collidable.h"

namespace ArkanoidGame
{
	class Ball final : public GameObject, public Collidable
	{
	public:
		Ball() = default;
		Ball(const sf::Vector2f& position);
		~Ball() = default;
		void Update(float timeDelta) override;
		void Draw(sf::RenderWindow& window) override;

		void InvertDirectionX();
		void InvertDirectionY();

		bool GetCollision(std::shared_ptr<Collidable> collidable) const override;
		void ChangeAngle(float x);

	private:
		void OnHit();
		sf::Vector2f direction;
		float lastAngle = 90;
	};
}