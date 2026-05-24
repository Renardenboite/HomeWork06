#pragma once
#include "SFML/Graphics.hpp"
#include "Menu.h"
#include "GameStateData.h"

namespace ArkanoidGame
{
	class Game;

	class GameStateMainMenuData : public GameStateData
	{
	public:
		void Init() override;
		void HandleWindowEvent(const sf::Event& event) override;
		void Update(float timeDelta) override;
		void Draw(sf::RenderWindow& window) override;

		/*GameStateMainMenuData() = default;

		GameStateMainMenuData(const GameStateMainMenuData&) = delete;
		GameStateMainMenuData& operator=(const GameStateMainMenuData&) = delete;

		GameStateMainMenuData(GameStateMainMenuData&&) = default;
		GameStateMainMenuData& operator=(GameStateMainMenuData&&) = default;*/

	private:
		sf::Font font;
		Menu menu;
	};
}
