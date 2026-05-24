#include "GameStatePlaying.h"
#include "Application.h"
#include "Game.h"
#include "Text.h"
#include <limits>
#include <random>
#include <assert.h>

namespace ArkanoidGame
{
    void GameStatePlayingData::Init()
    {
        const sf::Font& font = Application::Instance().GetGame().GetDefaultFont();
        assert(gameOverSoundBuffer.loadFromFile(SOUNDS_PATH + "Death.wav"));

        // ‘он
        background.setSize(sf::Vector2f(SCREEN_WIDTH, SCREEN_HEIGHT));
        background.setPosition(0.f, 0.f);
        background.setFillColor(sf::Color(30, 30, 30));

        // UI
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::Yellow);

        inputHintText.setFont(font);
        inputHintText.setCharacterSize(24);
        inputHintText.setFillColor(sf::Color::White);
        inputHintText.setString("Use arrow keys to move, ESC to pause");
        inputHintText.setOrigin(GetTextOrigin(inputHintText, { 1.f, 0.f }));

        // ѕлатформа
        auto platformObj = std::make_shared<Platform>();
        platformObj->Init();
        gameObjects.push_back(platformObj);
        platform = platformObj.get();

        // ћ€ч
        sf::Vector2f ballStartPos = {
            SCREEN_WIDTH / 2.f,
            SCREEN_HEIGHT - PLATFORM_HEIGHT - BALL_SIZE - 10.f
        };
        auto ballObj = std::make_shared<Ball>(ballStartPos);
        gameObjects.push_back(ballObj);
        ball = ballObj.get();

        // Ѕлоки
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

                if (row == 0)
                {
                    if (col == 0 || col == COLS - 1 || col == COLS / 2)
                        blockObj = std::make_shared<MultiHitBlock>(pos);
                    else
                    {
                        sf::Color color(colorDist(gen), colorDist(gen), colorDist(gen));
                        blockObj = std::make_shared<SmoothDestroyableBlock>(pos, color);
                    }
                }
                else if (row == 1)
                {
                    if (col == 0 || col == COLS - 1 || col == COLS / 2)
                        blockObj = std::make_shared<UnbreackableBlock>(pos);
                    else
                    {
                        sf::Color color(colorDist(gen), colorDist(gen), colorDist(gen));
                        blockObj = std::make_shared<SmoothDestroyableBlock>(pos, color);
                    }
                }
                else
                {
                    if (col == 1 || col == COLS - 2)
                        blockObj = std::make_shared<GlassBlock>(pos);
                    else if (col == COLS / 2)
                        blockObj = std::make_shared<SmoothDestroyableBlock>(pos, sf::Color::Magenta);
                    else
                    {
                        sf::Color color = (col % 2 == 0) ? sf::Color::Cyan : sf::Color(255, 200, 50);
                        blockObj = std::make_shared<SmoothDestroyableBlock>(pos, color);
                    }
                }

                gameObjects.push_back(blockObj);
                blocks.push_back(blockObj.get());

                if (!dynamic_cast<UnbreackableBlock*>(blockObj.get()))
                    totalDestroyableBlocks++;
            }
        }

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
        sf::Vector2f ballPos = ball->GetPosition();
        const float ballRadius = BALL_SIZE / 2.f;

        bool bounced = false;

        for (auto* block : blocks)
        {
            if (block->IsDestroyed()) continue;

            bool isGlass = dynamic_cast<GlassBlock*>(block) != nullptr;
            bool isUnbreackable = dynamic_cast<UnbreackableBlock*>(block) != nullptr;

            sf::FloatRect blockRect = block->GetRect();

            if (ballPos.x + ballRadius > blockRect.left &&
                ballPos.x - ballRadius < blockRect.left + blockRect.width &&
                ballPos.y + ballRadius > blockRect.top &&
                ballPos.y - ballRadius < blockRect.top + blockRect.height)
            {
                float fromLeft = ballPos.x + ballRadius - blockRect.left;
                float fromRight = blockRect.left + blockRect.width - (ballPos.x - ballRadius);
                float fromTop = ballPos.y + ballRadius - blockRect.top;
                float fromBottom = blockRect.top + blockRect.height - (ballPos.y - ballRadius);

                float minX = std::min(fromLeft, fromRight);
                float minY = std::min(fromTop, fromBottom);

                if (minX < minY)
                {
                    if (fromLeft < fromRight)
                        ballPos.x = blockRect.left - ballRadius - 5.f;
                    else
                        ballPos.x = blockRect.left + blockRect.width + ballRadius + 5.f;

                    if (!bounced && !isGlass)
                    {
                        ball->InvertDirectionX();
                        bounced = true;
                    }
                }
                else
                {
                    if (fromTop < fromBottom)
                        ballPos.y = blockRect.top - ballRadius - 5.f;
                    else
                        ballPos.y = blockRect.top + blockRect.height + ballRadius + 5.f;

                    if (!bounced && !isGlass)
                    {
                        ball->InvertDirectionY();
                        bounced = true;
                    }
                }

                ball->SetPosition(ballPos);
                ballPos = ball->GetPosition();

                if (!isUnbreackable)
                {
                    bool wasDestroyed = block->IsDestroyed();
                    block->OnHit();
                    bool isNowDestroyed = block->IsDestroyed();

                    if (!wasDestroyed && isNowDestroyed)
                    {
                        blocksDestroyed++;
                    }

                    auto* multiHit = dynamic_cast<MultiHitBlock*>(block);
                    if (multiHit && multiHit->IsBreaking() && !multiHit->IsCounted())
                    {
                        blocksDestroyed++;
                        multiHit->SetCounted(true);
                    }
                }

                //return;
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
        window.draw(background);

        for (auto& obj : gameObjects)
        {
            auto* block = dynamic_cast<Block*>(obj.get());
            if (block && block->IsDestroyed())
            {
                auto* delayed = dynamic_cast<IDelayedAction*>(obj.get());
                if (!delayed || !delayed->IsTimerStarted())
                    continue;
            }
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