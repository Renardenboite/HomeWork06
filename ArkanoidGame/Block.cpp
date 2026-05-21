#include "Block.h"
#include "GameSettings.h"
#include "Sprite.h"
#include <assert.h>

namespace
{
    const std::string TEXTURE_ID = "platform";
}

namespace SnakeGame
{
    void Block::Init()
    {
        assert(texture.loadFromFile(TEXTURES_PATH + TEXTURE_ID + ".png"));
        InitSprite(sprite, BLOCK_WIDTH, BLOCK_HEIGHT, texture);
    }

    void Block::Update(float timeDelta) {}
    void Block::Draw(sf::RenderWindow& window)
    {
        if (!destroyed) {
            DrawSprite(sprite, window);
        }
    }
}