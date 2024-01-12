#pragma once
#include "VoxelManager.hpp"

class GameEvent {
    public:
        virtual void enter() = 0;
        virtual void update(VoxelManager &vx) = 0;
        virtual void render(sf::RenderTarget&) = 0;

    private:
};