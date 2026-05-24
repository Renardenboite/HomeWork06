#pragma once
#include <SFML/Graphics.hpp>
#include "Menu.h"
#include "GameStateData.h"

namespace ArkanoidGame
{
	class Game;

	class GameStatePauseMenuData : public GameStateData
	{
	public:
		void Init() override;
		void HandleWindowEvent(const sf::Event& event) override;
		void Update(float timeDelta) override;
		void Draw(sf::RenderWindow& window) override;

		/*GameStatePauseMenuData() = default;
		GameStatePauseMenuData(const GameStatePauseMenuData&) = delete;
		GameStatePauseMenuData& operator=(const GameStatePauseMenuData&) = delete;
		GameStatePauseMenuData(GameStatePauseMenuData&&) = default;
		GameStatePauseMenuData& operator=(GameStatePauseMenuData&&) = default;*/

	private:
		sf::Font font;

		sf::RectangleShape background;
		sf::Text titleText;

		Menu menu;
	};
}
