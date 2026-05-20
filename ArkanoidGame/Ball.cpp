#include "Ball.h"
#include "GameSettings.h"
#include "Sprite.h"
#include <assert.h>

namespace
{
	// id textures
	const std::string TEXTURE_ID = "ball";
}

namespace SnakeGame
{
	void Ball::Init()
	{
		assert(texture.loadFromFile(TEXTURES_PATH + TEXTURE_ID + ".png"));

		InitSprite(sprite, BALL_SIZE, BALL_SIZE, texture);
		sprite.setPosition({ SCREEN_WIDTH / 2.0, SCREEN_HEIGHT - PLATFORM_HEIGHT / 2.f });

		const auto piAngle = std::acos(-1.f);
		direction.x = std::cos(piAngle / 180.f * angle);
		direction.y = std::sin(piAngle / 180.f * angle);
	}

	void Ball::Update(float timeDelta)
	{
		timeDelta = INITIAL_SPEED * timeDelta;
		const auto pos = sprite.getPosition() + INITIAL_SPEED * timeDelta * direction;
		sprite.setPosition(pos);

		if (pos.x - BALL_SIZE / 2.f <= 0 || pos.x + BALL_SIZE / 2.f >= SCREEN_WIDTH) {
			direction.x *= -1;
		}

		if (pos.y - BALL_SIZE / 2.f <= 0 || pos.y + BALL_SIZE / 2.f >= SCREEN_HEIGHT) {
			direction.y *= -1;
		}
	}

	void Ball::Draw(sf::RenderWindow& window)
	{
		DrawSprite(sprite, window);
	}

	void Ball::ReboundFromPlatform()
	{
		float lastAngle = angle;
		const auto pAngle = std::acos(-1.f);
		direction.x = (angle / abs(angle)) * std::cos(pAngle / 180.f * angle);
		direction.y = -1 * abs(std::sin(pAngle / 180.f * angle));
	}
}