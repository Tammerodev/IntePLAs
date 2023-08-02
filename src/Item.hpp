#pragma once
#include <SFML/Graphics.hpp>
#include "VoxelManager.hpp"

class Item {
public:
    virtual void use(const sf::Vector2f&,const sf::Vector2f&, World &world) = 0;
    virtual void render(sf::RenderTarget&) = 0;
    virtual void update(World&, const sf::Vector2f&, const sf::Vector2f&, const float) = 0;
    virtual void setPosition(const sf::Vector2f&) = 0;
    virtual sf::Sprite &getSprite() = 0;
};