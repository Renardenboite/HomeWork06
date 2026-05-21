#pragma once
#include <SFML/Graphics.hpp>
#include "GameStateData.h"


namespace SnakeGame
{
	enum class GameStateType
	{
		None = 0,
		MainMenu,
		Playing,
		GameOver,
		ExitDialoge,
		Records,
		Victory,
	};

	class GameState
	{
	public:
		GameState() = default;
		GameState(GameStateType type, bool isExclusivelyVisible);
		GameState(const GameState& state) = delete;
		GameState(GameState&& state) noexcept { operator=(std::move(state)); }
		/*GameState(GameState&& state) noexcept
			: type(state.type)
			, data(state.data)
			, isExclusivelyVisible(state.isExclusivelyVisible)
		{
			state.data = nullptr;
		}*/

		~GameState();

		GameState& operator= (const GameState& state) = delete;
		GameState& operator= (GameState&& state) noexcept
		{
			type = state.type;
			data = std::move(state.data);
			isExclusivelyVisible = state.isExclusivelyVisible;
			state.data = nullptr;
			return *this;
		}

		GameStateType GetType() const { return type; }
		bool IsExclusivelyVisible() const { return isExclusivelyVisible; }

		template<class T>
		T* GetData() const
		{
			return static_cast<T>(data);
		}

		void Update(float timeDelta);
		void Draw(sf::RenderWindow& window);
		void HandleWindowEvent(sf::Event& event);

		/*private:
			void* CopyData(const GameState& state) const;*/

	private:
		GameStateType type = GameStateType::None;
		std::unique_ptr<GameStateData> data = nullptr;
		bool isExclusivelyVisible = false;
	};
}