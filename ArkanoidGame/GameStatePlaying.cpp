#include "GameStatePlaying.h"
#include "Application.h"
#include "Game.h"
#include "Text.h"
#include <limits>
#include <windows.h>
#include <random>
#include <assert.h>
#include <sstream>

namespace SnakeGame
{
	void GameStatePlayingData::Init()
	{	
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

		auto platformObj = std::make_shared<Platform>();
		platformObj->Init();
		gameObjects.push_back(platformObj);
		platform = platformObj.get();

		auto ballObj = std::make_shared<Ball>();
		ballObj->Init();
		gameObjects.push_back(ballObj);
		ball = ballObj.get();

		const float totalWidth = COLS * BLOCK_WIDTH + (COLS - 1) * blockSpacingX;
		const float startX = (SCREEN_WIDTH - totalWidth) / 2.f;
		const float startY = 80.f;
		for (int row = 0; row < ROWS; ++row)
		{
			for (int col = 0; col < COLS; ++col)
			{
				auto blockObj = std::make_shared<Block>();
				blockObj->Init();

				float x = startX + col * (BLOCK_WIDTH + blockSpacingX);
				float y = startY + row * (BLOCK_HEIGHT + blockSpacingY);
				blockObj->SetPosition({ x, y });

				gameObjects.push_back(blockObj);
				blocks.push_back(blockObj.get());
			}
		}

		// Init sounds
		gameOverSound.setBuffer(gameOverSoundBuffer);

		blocksDestroyed = 0;
		gameWon = false;
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
		if (gameWon) return;

		for (auto& obj : gameObjects)
		{
			obj->Update(timeDelta);
		}

		if (platform->CheckCollisionWithBall(*ball))
		{
			ball->ReboundFromPlatform();
		}

		CheckBallCollisionWithBlocks();

		CheckWinCondition();

		if (ball->GetPosition().y > SCREEN_HEIGHT + BALL_SIZE)
		{
			gameOverSound.play();
			Game& game = Application::Instance().GetGame();
			game.PushState(GameStateType::GameOver, false);
		}		
	}

	void GameStatePlayingData::CheckBallCollisionWithBlocks()
	{
		const sf::Vector2f ballPos = ball->GetPosition();
		const float ballRadius = BALL_SIZE / 2.f;

		Block* closestBlock = nullptr;
		float minDistance = std::numeric_limits<float>::max();
		sf::FloatRect closestRect;
		bool hitSide = false;  

		std::vector<Block*> collidedBlocks;

		for (auto* block : blocks)
		{
			if (block->IsDestroyed()) continue;

			const sf::FloatRect blockRect = block->GetRect();

			float overlapLeft = (ballPos.x + ballRadius) - blockRect.left;
			float overlapRight = (blockRect.left + blockRect.width) - (ballPos.x - ballRadius);
			float overlapTop = (ballPos.y + ballRadius) - blockRect.top;
			float overlapBottom = (blockRect.top + blockRect.height) - (ballPos.y - ballRadius);

			if (overlapLeft > 0 && overlapRight > 0 && overlapTop > 0 && overlapBottom > 0)
			{				
				collidedBlocks.push_back(block);

				float blockCenterX = blockRect.left + blockRect.width / 2.f;
				float blockCenterY = blockRect.top + blockRect.height / 2.f;
				float distX = ballPos.x - blockCenterX;
				float distY = ballPos.y - blockCenterY;
				float distance = std::sqrt(distX * distX + distY * distY);

				if (distance < minDistance)
				{
					minDistance = distance;
					closestBlock = block;
					closestRect = blockRect;

					float minOverlapX = std::min(overlapLeft, overlapRight);
					float minOverlapY = std::min(overlapTop, overlapBottom);
					hitSide = (minOverlapX < minOverlapY);
				}				
			}			
		}

		if (!collidedBlocks.empty())
		{
			if (hitSide)
			{
				ball->ReboundFromBlockSide();
			}
			else
			{
				ball->ReboundFromBlock();
			}

			for (auto* block : collidedBlocks)
			{
				block->Destroy();
				blocksDestroyed++;
			}
		}
	}

	void GameStatePlayingData::CheckWinCondition()
	{
		if (blocksDestroyed >= blocks.size())
		{
			gameWon = true;

			Game& game = Application::Instance().GetGame();
			game.PushState(GameStateType::Victory, false);  
		}
	}

	void GameStatePlayingData::Draw(sf::RenderWindow& window)
	{
		// Draw background
		window.draw(background);

		for (auto& obj : gameObjects)
		{
			auto* block = dynamic_cast<Block*>(obj.get());
			if (block && block->IsDestroyed()) continue;  // Пропускаем уничтоженные блоки

			obj->Draw(window);
		}

		scoreText.setOrigin(GetTextOrigin(scoreText, { 0.f, 0.f }));
		scoreText.setPosition(10.f, 10.f);
		window.draw(scoreText);

		sf::Vector2f viewSize = window.getView().getSize();
		inputHintText.setPosition(viewSize.x - 10.f, 10.f);
		window.draw(inputHintText);
	}
}
