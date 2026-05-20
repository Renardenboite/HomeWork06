#pragma once
#include "SFML/Graphics.hpp"

namespace SnakeGame
{
	class Game;

	class GameStateGameOverData
	{
	public:
		void Init();
		void HandleWindowEvent(const sf::Event& event);
		void Update(float timeDelta);
		void Draw(sf::RenderWindow& window);

		GameStateGameOverData() = default;
		GameStateGameOverData(const GameStateGameOverData&) = delete;
		GameStateGameOverData& operator=(const GameStateGameOverData&) = delete;
		GameStateGameOverData(GameStateGameOverData&&) = default;
		GameStateGameOverData& operator=(GameStateGameOverData&&) = default;

	private:
		sf::Font font;

		float timeSinceGameOver = 0.f;

		// UI data
		sf::RectangleShape background;
		sf::Text gameOverText;
		sf::Text hintText;
		std::vector<std::unique_ptr<sf::Text>> recordsTableTexts;
		
	};	
}
