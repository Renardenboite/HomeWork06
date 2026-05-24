#include "GameStatePlaying.h"
#include "Application.h"
#include "Game.h"
#include "Text.h"
#include <limits>
#include <windows.h>
#include <random>
#include <assert.h>
#include <sstream>

namespace ArkanoidGame
{
	void GameStatePlayingData::Init()
	{	
		const sf::Font& font = Application::Instance().GetGame().GetDefaultFont();
		assert(gameOverSoundBuffer.loadFromFile(SOUNDS_PATH + "Death.wav"));

		// Init background
		background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
		background.setPosition(0.f, 0.f);
		background.setFillColor(sf::Color(30, 30, 30));

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

		sf::Vector2f ballStartPos = {
			SCREEN_WIDTH / 2.f,
			SCREEN_HEIGHT - PLATFORM_HEIGHT - BALL_SIZE - 10.f
		};

		auto ballObj = std::make_shared<Ball>(ballStartPos);
		gameObjects.push_back(ballObj);
		ball = ballObj.get();

		const float totalWidth = COLS * BLOCK_WIDTH + (COLS - 1) * blockSpacingX;
		const float startX = (SCREEN_WIDTH - totalWidth) / 2.f;
		const float startY = 80.f;

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<int> colorDist(0, 255);

		for (int row = 0; row < ROWS; ++row)
		{
			for (int col = 0; col < COLS; ++col)
			{
				float x = startX + col * (BLOCK_WIDTH + blockSpacingX);
				float y = startY + row * (BLOCK_HEIGHT + blockSpacingY);
				sf::Vector2f pos = { x, y };

				std::shared_ptr<Block> blockObj;

				//high row
				if (row == 0)
				{
					if (col == 0 || col == COLS - 1)
					{						
						blockObj = std::make_shared<MultiHitBlock>(pos);
					}
					else if (col == COLS / 2)
					{						
						blockObj = std::make_shared<MultiHitBlock>(pos);
					}
					else
					{
						sf::Color color(colorDist(gen), colorDist(gen), colorDist(gen));
						blockObj = std::make_shared<SmoothDestroyableBlock>(pos, color);
					}
				}

				//middle row
				else if (row == 1)
				{
					if (col == 0 || col == COLS - 1 || col == COLS / 2)
					{
						blockObj = std::make_shared<UnbreackableBlock>(pos);
					}
					else
					{
						sf::Color color(colorDist(gen), colorDist(gen), colorDist(gen));
						blockObj = std::make_shared<SmoothDestroyableBlock>(pos, color);
					}
				}

				//low row
				else
				{
					if (col == 1 || col == COLS - 2)
					{
						blockObj = std::make_shared<GlassBlock>(pos);
					}
					else if (col == COLS / 2)
					{
						// ÷ентральный Ч €ркий обычный блок
						blockObj = std::make_shared<SmoothDestroyableBlock>(pos, sf::Color::Magenta);
					}
					else
					{
						// ќбычные блоки
						sf::Color color = (col % 2 == 0) ? sf::Color::Cyan : sf::Color(255, 200, 50);
						blockObj = std::make_shared<SmoothDestroyableBlock>(pos, color);
					}
				}

				gameObjects.push_back(blockObj);
				blocks.push_back(blockObj.get());

				if (!dynamic_cast<UnbreackableBlock*>(blockObj.get()))
				{
					totalDestroyableBlocks++;
				}				
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
			ball->InvertDirectionY();
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

		struct CollisionInfo
		{
			Block* block;
			float distance;
			float overlapX;
			float overlapY;
			bool hitSide;
		};
		std::vector<CollisionInfo> collisions;

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
				//collidedBlocks.push_back(block);

				float blockCenterX = blockRect.left + blockRect.width / 2.f;
				float blockCenterY = blockRect.top + blockRect.height / 2.f;
				float distX = ballPos.x - blockCenterX;
				float distY = ballPos.y - blockCenterY;
				float distance = std::sqrt(distX * distX + distY * distY);

				float minOverlapX = std::min(overlapLeft, overlapRight);
				float minOverlapY = std::min(overlapTop, overlapBottom);
				collisions.push_back({
					block,
					distance,
					overlapLeft,
					overlapTop,
					(minOverlapX < minOverlapY) 
					});								
			}			
		}

		if (collisions.empty()) return;

		auto closestIt = std::min_element(collisions.begin(), collisions.end(),
			[](const CollisionInfo& a, const CollisionInfo& b) {
				return a.distance < b.distance;
			});

		bool hasGlassBlock = false;
		for (const auto& col : collisions)
		{
			if (dynamic_cast<GlassBlock*>(col.block))
			{
				hasGlassBlock = true;
				break;
			}
		}

		if (!hasGlassBlock)
		{
			if (closestIt->hitSide)
			{
				ball->InvertDirectionX();
			}
			else
			{
				ball->InvertDirectionY();
			}

			sf::Vector2f newBallPos = ball->GetPosition();
			const sf::FloatRect& closestRect = closestIt->block->GetRect();

			if (closestIt->hitSide)
			{
				if (newBallPos.x < closestRect.left + closestRect.width / 2.f)
					newBallPos.x = closestRect.left - ballRadius;
				else
					newBallPos.x = closestRect.left + closestRect.width + ballRadius;
			}
			else
			{
				if (newBallPos.y < closestRect.top + closestRect.height / 2.f)
					newBallPos.y = closestRect.top - ballRadius;
				else
					newBallPos.y = closestRect.top + closestRect.height + ballRadius;
			}
			ball->SetPosition(newBallPos);
		}

		for (auto& col : collisions)
		{
			col.block->OnHit();

			if (col.block->IsDestroyed() && !dynamic_cast<UnbreackableBlock*>(col.block))
			{
				blocksDestroyed++;
			}
		}	
	}

	void GameStatePlayingData::CheckWinCondition()
	{
		if (blocksDestroyed >= totalDestroyableBlocks)
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
			if (block && block->IsDestroyed()) continue; 

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
