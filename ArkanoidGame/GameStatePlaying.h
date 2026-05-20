#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"

#include "Ball.h"
#include "Platform.h"


namespace SnakeGame
{
	class GameStatePlayingData
	{
	public:
		void Init();
		void HandleWindowEvent(const sf::Event& event);
		void Update(float timeDelta);
		void Draw(sf::RenderWindow& window);

		GameStatePlayingData() = default;
		GameStatePlayingData(const GameStatePlayingData&) = delete;
		GameStatePlayingData& operator=(const GameStatePlayingData&) = delete;
		GameStatePlayingData(GameStatePlayingData&&) = default;
		GameStatePlayingData& operator=(GameStatePlayingData&&) = default;

	private:
		// Resources
		sf::Font font;
		sf::SoundBuffer gameOverSoundBuffer;

		// UI data
		sf::Text scoreText;
		sf::Text inputHintText;
		sf::RectangleShape background;

		// Sounds
		sf::Sound gameOverSound;

		Platform platform;
		Ball ball;
	};
}
