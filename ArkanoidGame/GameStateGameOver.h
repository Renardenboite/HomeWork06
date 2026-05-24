#pragma once
#include "SFML/Graphics.hpp"
#include "GameStateData.h"

namespace ArkanoidGame
{
	class Game;

	class GameStateGameOverData : public GameStateData
	{
	public:
		void Init() override;
		void HandleWindowEvent(const sf::Event& event) override;
		void Update(float timeDelta) override;
		void Draw(sf::RenderWindow& window) override;

		/*GameStateGameOverData() = default;
		GameStateGameOverData(const GameStateGameOverData&) = delete;
		GameStateGameOverData& operator=(const GameStateGameOverData&) = delete;
		GameStateGameOverData(GameStateGameOverData&&) = default;
		GameStateGameOverData& operator=(GameStateGameOverData&&) = default;*/

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
