#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

namespace SnakeGame
{
	class Game;

	class GameStateRecordsData
	{
	public:
		void Init();
		void HandleWindowEvent(const sf::Event& event);
		void Update(float timeDelta);
		void Draw(sf::RenderWindow& window);

		GameStateRecordsData() = default;
		GameStateRecordsData(const GameStateRecordsData&) = delete;
		GameStateRecordsData& operator=(const GameStateRecordsData&) = delete;
		GameStateRecordsData(GameStateRecordsData&&) = default;
		GameStateRecordsData& operator=(GameStateRecordsData&&) = default;

	private:
		sf::Font font;

		sf::Text titleText;
		std::vector<std::unique_ptr<sf::Text>> tableTexts;
		sf::Text hintText;
	};
}