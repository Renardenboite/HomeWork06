#pragma once
#include "GameObject.h"

namespace SnakeGame
{
    class Block : public GameObject
    {
    public:
        void Init() override;
        void Update(float timeDelta) override;
        void Draw(sf::RenderWindow& window) override;

        bool IsDestroyed() const { return destroyed; }
        void Destroy() { destroyed = true; }

    private:
        bool destroyed = false;
    };
}