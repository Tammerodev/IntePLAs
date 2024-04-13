#pragma once
#include "../../Entity.hpp"

class Helicopter : public EntityComponents {
public:
    void load() {
        texture.loadFromFile("res/mod/img/helicopter.png");
        sprite.setTexture(texture);

        sprite.setPosition(-500, 0);
    }
    
    void update(const float dt, Game& game, const sf::Vector2f& playerPosition) {
        sf::Vector2f target = playerPosition;
        sf::Vector2f movement = target - sprite.getPosition();
        movement = math::normalize(movement) * 150.f;

        sf::Vector2i pos_i = sf::Vector2i(sprite.getPosition());

        if(game.world.main_world.getChunkIndexer().doesLineContainMaterial(pos_i, pos_i + sf::Vector2i(40, 40))) {
            movement.y = -0.5;
            movement.x = -0.5;
        }

        if(game.world.main_world.getChunkIndexer().doesLineContainMaterial(pos_i, pos_i - sf::Vector2i(40, 40))) {
            movement.y = 0.5;
            movement.x = 0.5;
        }

        if(sprite.getPosition().y > minAltitude) 
            movement.y = -0.5;

        sprite.move(movement / 100.f);

        frame++;
        if(frame > 5)
            frame = 0;

        sf::IntRect textureRect(
            sf::Vector2i(0, frame * 32),
            sf::Vector2i(84, 32)
        );

        sprite.setTextureRect(textureRect);
    }

    const sf::Vector2f& getPosition() {
        return sprite.getPosition();
    }

    const sf::Vector2f getTurretPosition() {
        return sprite.getPosition() + sf::Vector2f(14.f, 24.f);
    }

    void render(sf::RenderTarget& target) {
        sprite.setTexture(texture);
        target.draw(sprite);
    }

    int ammo = max_ammo;
    const int max_ammo = 15;
private:
    int minAltitude = 500;
    sf::Vector2f position;

    int frame = 0;

};