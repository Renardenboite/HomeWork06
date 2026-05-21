#include "GameState.h"
#include "GameStateVictory.h"
#include "GameStateData.h"
#include "GameStatePlaying.h"
#include "GameStateGameOver.h"
#include "GameStateMainMenu.h"
#include "GameStatePauseMenu.h"
#include "GameStateRecords.h"
#include <assert.h>


namespace SnakeGame
{
	GameState::GameState(GameStateType type, bool isExclusivelyVisible)
		: type(type)
		, isExclusivelyVisible(isExclusivelyVisible)
	{
		switch (type)
		{
		case GameStateType::MainMenu:
		{
			data = std::make_unique<GameStateMainMenuData>();
			break;
		}
		case GameStateType::Playing:
		{
			data = std::make_unique<GameStatePlayingData>();			
			break;
		}
		case GameStateType::GameOver:
		{
			data = std::make_unique<GameStateGameOverData>();			
			break;
		}
		case GameStateType::ExitDialoge:
		{
			data = std::make_unique<GameStatePauseMenuData>();			
			break;
		}
		case GameStateType::Records:
		{
			data = std::make_unique<GameStateRecordsData>();
			break;
		}
		case GameStateType::Victory:
		{
			data = std::make_unique<GameStateVictoryData>();
			break;
		}
		default:
			assert(false);
			break;		
		}
		if (data)
		{
			data->Init();
		}
	}

	void GameState::Update(float timeDelta)
	{
		data->Update(timeDelta);
	}

	void GameState::Draw(sf::RenderWindow& window)
	{
		data->Draw(window);
	}

	void GameState::HandleWindowEvent(sf::Event& event)
	{
		data->HandleWindowEvent(event);
	}

	GameState::~GameState()
	{
		if (data)
		{
			data = nullptr;
		}
	}
}