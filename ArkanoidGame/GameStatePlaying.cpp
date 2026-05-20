#include "GameStatePlaying.h"
#include "Application.h"
#include "Game.h"
#include "Text.h"

#include <windows.h>

#include <assert.h>
#include <sstream>

namespace SnakeGame
{
	void GameStatePlayingData::Init()
	{	
		// Init game resources (terminate if error)
		//assert(font.loadFromFile(FONTS_PATH + "Roboto-Regular.ttf"));
		const sf::Font& font = Application::Instance().GetGame().GetDefaultFont();

		assert(gameOverSoundBuffer.loadFromFile(SOUNDS_PATH + "Death.wav"));

		// Init background
		background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
		background.setPosition(0.f, 0.f);
		background.setFillColor(sf::Color(0, 200, 0));

		scoreText.setFont(font);
		scoreText.setCharacterSize(24);
		scoreText.setFillColor(sf::Color::Yellow);

		inputHintText.setFont(font);
		inputHintText.setCharacterSize(24);
		inputHintText.setFillColor(sf::Color::White);
		inputHintText.setString("Use arrow keys to move, ESC to pause");
		inputHintText.setOrigin(GetTextOrigin(inputHintText, { 1.f, 0.f }));

		platform.Init();
		ball.Init();

		// Init sounds
		gameOverSound.setBuffer(gameOverSoundBuffer);
	}

	void GameStatePlayingData::HandleWindowEvent(const sf::Event& event)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				Application::Instance().GetGame().PushState(GameStateType::ExitDialoge, false);
			}
		}
	}

	void GameStatePlayingData::Update(float timeDelta)
	{
		static int frame = 0;
		if (frame++ < 10) {
			std::string msg = "Playing Update frame " + std::to_string(frame) +
				", ball Y: " + std::to_string(ball.GetPosition().y) + "\n";
			OutputDebugStringA(msg.c_str());
		}

		platform.Update(timeDelta);
		ball.Update(timeDelta);
		const bool isCollision = platform.CheckCollisionWithBall(ball);
		if (isCollision)
		{
			ball.ReboundFromPlatform();
		}
		
		const bool isGameFinished = ball.GetPosition().y > SCREEN_HEIGHT + BALL_SIZE;

		if (isGameFinished)
		{
			gameOverSound.play();
			
			Game& game = Application::Instance().GetGame();

			// Find snake in records table and update his score
			//game.UpdateRecord(PLAYER_NAME, numEatenApples);
			game.PushState(GameStateType::GameOver, false);
		}
	}

	void GameStatePlayingData::Draw(sf::RenderWindow& window)
	{
		// Draw background
		window.draw(background);

		platform.Draw(window);
		ball.Draw(window);

		scoreText.setOrigin(GetTextOrigin(scoreText, { 0.f, 0.f }));
		scoreText.setPosition(10.f, 10.f);
		window.draw(scoreText);

		sf::Vector2f viewSize = window.getView().getSize();
		inputHintText.setPosition(viewSize.x - 10.f, 10.f);
		window.draw(inputHintText);
	}
}
