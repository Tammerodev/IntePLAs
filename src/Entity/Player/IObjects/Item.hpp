#pragma once
#include <SFML/Graphics.hpp>
#include "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/src/VoxelWorld/VoxelManager.hpp"

class Item {
public:
    virtual void use(const sf::Vector2f&,const sf::Vector2f&) = 0;
    virtual void render(sf::RenderTarget&) = 0;
    virtual void update(VoxelManager&, const sf::Vector2f&, const sf::Vector2f&, const float) = 0;
    virtual void setPosition(const sf::Vector2f&) = 0;
    virtual sf::Sprite &getSprite() = 0;
};