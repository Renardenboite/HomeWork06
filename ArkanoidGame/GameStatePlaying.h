#pragma once
#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "GameStateData.h"
#include "Ball.h"
#include "Platform.h"
#include "Block.h"


namespace ArkanoidGame
{
	class Game;

	class GameStatePlayingData : public GameStateData
	{
	public:
		void Init() override;
		void HandleWindowEvent(const sf::Event& event) override;
		void Update(float timeDelta) override;
		void Draw(sf::RenderWindow& window) override;

		
	private:
		void CheckBallCollisionWithBlocks();
		void CheckWinCondition();

		// Resources
		sf::Font font;
		sf::SoundBuffer gameOverSoundBuffer;

		// Game data
		std::vector<std::shared_ptr<GameObject>> gameObjects;

		// UI data
		sf::Text scoreText;
		sf::Text inputHintText;
		sf::RectangleShape background;

		// Sounds
		sf::Sound gameOverSound;

		Platform* platform = nullptr;
		Ball* ball = nullptr;
		std::vector<Block*> blocks; 

		int blocksDestroyed = 0;
		int totalDestroyableBlocks = 0;
		bool gameWon = false;		
	};
}
