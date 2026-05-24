#include "Block.h"
#include "GameSettings.h"
#include "Sprite.h"
#include <assert.h>

namespace
{
    const std::string TEXTURE_ID = "block_multi_0";
}

namespace ArkanoidGame
{
    Block::Block(const sf::Vector2f& position, const sf::Color& color)
        : GameObject(TEXTURES_PATH + TEXTURE_ID + ".png", position, BLOCK_WIDTH, BLOCK_HEIGHT)
    {
        sprite.setColor(color);
    }

    bool Block::GetCollision(std::shared_ptr<Collidable> collidableObject) const
    {
        auto gameObject = std::dynamic_pointer_cast<GameObject>(collidableObject);
        assert(gameObject);
        sf::Rect rect = gameObject->GetRect();
        rect.width *= 1.1f;
        return GetRect().intersects(gameObject->GetRect());
    }

    void Block::Draw(sf::RenderWindow& window)
    {
        if (!IsDestroyed())
        {
            DrawSprite(sprite, window);
        }
    }

    void Block::OnHit()
    {
        hitCount = 0;
    }

    /*bool Block::IsDestroyed() const
    {
        return hitCount <= 0;
    }*/

    void Block::Update(float timeDelta) {}

    Block::~Block() {};

    SmoothDestroyableBlock::SmoothDestroyableBlock(const sf::Vector2f& position, const sf::Color& color)
        : Block(position, color)
        , color(color)
    {
    }

    void SmoothDestroyableBlock::Update(float timeDelta)
    {
        UpdateTimer(timeDelta);
    }

    bool SmoothDestroyableBlock::GetCollision(std::shared_ptr<Collidable> collidableObject) const
    {
        if (isTimerStarted_)
        {
            return false;
        }

        auto gameObject = std::dynamic_pointer_cast<GameObject>(collidableObject);
        assert(gameObject);
        sf::Rect rect = gameObject->GetRect();
        rect.width *= 1.1f;
        return GetRect().intersects(gameObject->GetRect());
    }

    void SmoothDestroyableBlock::OnHit()
    {
        StartTimer(BREAK_DELAY);
    }

    void SmoothDestroyableBlock::FinalAction()
    {
        --hitCount;
    }

    void SmoothDestroyableBlock::EachTickAction(float timeDelta)
    {
        color.a = static_cast<sf::Uint8>(255 * currentTime_ / destroyTime_);
        sprite.setColor(color);
    }

    UnbreackableBlock::UnbreackableBlock(const sf::Vector2f& position)
        : Block(position, sf::Color::Color(105, 105, 105))
    { }

    void UnbreackableBlock::OnHit()
    {
    }

    MultiHitBlock::MultiHitBlock(const sf::Vector2f& position)
        : Block(position, sf::Color::Green)
    {
        maxHits = 3;
        currentHits = 0;
        originalColor = sf::Color::Green;

        sf::Texture tex0, tex1, tex2, tex3;
        tex0.loadFromFile(TEXTURES_PATH + "block_multi_0.png"); 
        tex1.loadFromFile(TEXTURES_PATH + "block_multi_1.png"); 
        tex2.loadFromFile(TEXTURES_PATH + "block_multi_2.png"); 
        tex3.loadFromFile(TEXTURES_PATH + "block_multi_3.png");

        hitTextures.push_back(tex0);
        hitTextures.push_back(tex1);
        hitTextures.push_back(tex2);
        hitTextures.push_back(tex3);

        sprite.setTexture(hitTextures[0]);
    }

    void MultiHitBlock::Update(float timeDelta)
    {
        if (isBreaking)
        {
            UpdateTimer(timeDelta); 
        }
    }

    void MultiHitBlock::OnHit()
    {
        currentHits++;

        if (currentHits < maxHits)
        {
            UpdateTexture(); 
        }
        else if (currentHits == maxHits)
        {
            UpdateTexture();
            isBreaking = true;
            StartTimer(BREAK_DELAY);
        }
    }

    void MultiHitBlock::UpdateTexture()
    {
        if (currentHits < static_cast<int>(hitTextures.size()))
        {
            sprite.setTexture(hitTextures[currentHits]);
        }
    }

    void MultiHitBlock::EachTickAction(float timeDelta)
    {
        float alpha = originalColor.a * (currentTime_ / destroyTime_);
        sprite.setColor(sf::Color(
            originalColor.r,
            originalColor.g,
            originalColor.b,
            static_cast<sf::Uint8>(alpha)
        ));
    }

    void MultiHitBlock::FinalAction()
    {
        hitCount = 0; 
    }

    bool MultiHitBlock::IsDestroyed() const
    {
        return hitCount <= 0;
    }


    GlassBlock::GlassBlock(const sf::Vector2f& position)
        : Block(position, sf::Color(200, 220, 255, 180))
    {
        hitCount = 1;
        originalColor = sf::Color(200, 220, 255, 180);
    }

    void GlassBlock::Update(float timeDelta)
    {
        if (collisionProcessed)
        {
            UpdateTimer(timeDelta);
        }
    }

    bool GlassBlock::GetCollision(std::shared_ptr<Collidable> collidableObject) const
    {
        if (collisionProcessed) return false;

        auto gameObject = std::dynamic_pointer_cast<GameObject>(collidableObject);
        assert(gameObject);
        return GetRect().intersects(gameObject->GetRect());
    }

    void GlassBlock::OnHit()
    {
        if (!collisionProcessed)
        {
            collisionProcessed = true;
            StartTimer(BREAK_DELAY);  
        }
    }

    void GlassBlock::EachTickAction(float timeDelta)
    {
        float alpha = originalColor.a * (currentTime_ / destroyTime_);
        sprite.setColor(sf::Color(
            originalColor.r,
            originalColor.g,
            originalColor.b,
            static_cast<sf::Uint8>(alpha)
        ));
    }

    void GlassBlock::FinalAction()
    {
        hitCount = 0;
    }

    bool GlassBlock::IsDestroyed() const
    {
        return hitCount <= 0;
    }
}