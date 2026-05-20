#pragma once
#include <SFML/Graphics.hpp>
#include "Menu.h"

namespace SnakeGame
{
	class Game;

	class GameStatePauseMenuData
	{
	public:
		void Init();
		void HandleWindowEvent(const sf::Event& event);
		void Update(float timeDelta);
		void Draw(sf::RenderWindow& window);

		GameStatePauseMenuData() = default;
		GameStatePauseMenuData(const GameStatePauseMenuData&) = delete;
		GameStatePauseMenuData& operator=(const GameStatePauseMenuData&) = delete;
		GameStatePauseMenuData(GameStatePauseMenuData&&) = default;
		GameStatePauseMenuData& operator=(GameStatePauseMenuData&&) = default;

	private:
		sf::Font font;

		sf::RectangleShape background;
		sf::Text titleText;

		Menu menu;
	};
}
