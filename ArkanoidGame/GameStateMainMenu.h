#pragma once
#include "SFML/Graphics.hpp"
#include "Menu.h"

namespace SnakeGame
{
	class Game;

	class GameStateMainMenuData
	{
	public:
		void Init();
		void HandleWindowEvent(const sf::Event& event);
		void Update(float timeDelta);
		void Draw(sf::RenderWindow& window);

		GameStateMainMenuData() = default;

		GameStateMainMenuData(const GameStateMainMenuData&) = delete;
		GameStateMainMenuData& operator=(const GameStateMainMenuData&) = delete;

		GameStateMainMenuData(GameStateMainMenuData&&) = default;
		GameStateMainMenuData& operator=(GameStateMainMenuData&&) = default;

	private:
		sf::Font font;
		Menu menu;
	};
}
