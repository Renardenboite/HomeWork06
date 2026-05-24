#pragma once
#include "GameObject.h"
#include "Collidable.h"
#include "IDelayedAction.h"
#include <vector>

namespace ArkanoidGame
{
    class Block : public GameObject, public Collidable
    {     
    public:
        Block() = default;
        Block(const sf::Vector2f& position, const sf::Color& color = sf::Color::Red);
        virtual ~Block();
        void Draw(sf::RenderWindow& window) override;
        void Update(float timeDelta) override;        
        bool GetCollision(std::shared_ptr<Collidable> collidableObject) const override;
        virtual bool IsDestroyed() const { return hitCount <= 0; }
        virtual void OnHit();
        int hitCount = 1;
    protected:
        bool destroyed = false;  
        
    };

    class SmoothDestroyableBlock : public Block, public IDelayedAction
    {
    protected:
        void OnHit() override;
        sf::Color color;
    public:
        SmoothDestroyableBlock(const sf::Vector2f& position, const sf::Color& color = sf::Color::Blue);
        ~SmoothDestroyableBlock() = default;
        void Update(float timeDelta) override;

        bool GetCollision(std::shared_ptr<Collidable> collidableObject) const override;
        void FinalAction() override;
        void EachTickAction(float deltaTime) override;
    };

    class UnbreackableBlock : public Block
    {
    public:
        UnbreackableBlock(const sf::Vector2f& position);
        void OnHit() override;
        void Update(float timeDelta) override {} 
    };

    class MultiHitBlock : public Block, public IDelayedAction
    {
    public:
        MultiHitBlock(const sf::Vector2f& position);
        void OnHit() override;
        bool IsDestroyed() const override;
        void Update(float timeDelta) override;

        void FinalAction() override;
        void EachTickAction(float deltaTime) override;

    private:
        int maxHits = 3;
        int currentHits = 0;
        std::vector<sf::Texture> hitTextures;
        sf::Color originalColor;

        void UpdateTexture();
        bool isBreaking = false; 
    };

    class GlassBlock : public Block, public IDelayedAction
    {
    public:
        GlassBlock(const sf::Vector2f& position);
        bool GetCollision(std::shared_ptr<Collidable> collidableObject) const override;
        void OnHit() override;
        bool IsDestroyed() const override;
        void Update(float timeDelta) override;

        void FinalAction() override;
        void EachTickAction(float deltaTime) override;

    private:
        mutable bool collisionProcessed = false;
        sf::Color originalColor;
    };
}