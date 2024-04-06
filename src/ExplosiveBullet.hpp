#pragma once
#include <SFML/Graphics.hpp>

class ExplosiveBullet {
public:
    ExplosiveBullet(const sf::Texture &bullet_tx, const sf::Vector2f &pos) {
        setPosition(pos);
        spr.setPosition(pos);
        spr.setTexture(bullet_tx);
        spr.setOrigin(sf::Vector2f(bullet_tx.getSize() / 2u));
    
        prev_pos = pos;
    }

    virtual ~ExplosiveBullet() {
        
    }

    void update(const float dt) {
        float rotation = std::atan2(vel.y, vel.x) * (180.0 / math::PI);
        current_rotation += (rotation - current_rotation) / 30.f;
        spr.setRotation(current_rotation);

        prev_pos = spr.getPosition();

        vel.y += PhysicsGlobals::gravity / 10.f;

        timer++;

        pos += vel * speed * dt;
        spr.setPosition(pos);
    }

    void draw(sf::RenderTarget &target) {
        target.draw(spr);
    }

    void setPosition(sf::Vector2f pos) {
        this->pos = pos;
    }

    void setVelocity(sf::Vector2f vel) {
        this->vel = vel;
    }

    void setRotation(const float& angle) {
        spr.setRotation(angle);
        current_rotation = angle;
    }

    sf::FloatRect getCollider() {
        return spr.getGlobalBounds();
    }

    sf::Vector2f pos = sf::Vector2f(0, 0);
    sf::Vector2f prev_pos = sf::Vector2f(0, 0);

    float current_rotation = 0.f;

    int timer = 0;

    sf::Vector2f vel = sf::Vector2f(0,0);
private:
    sf::Sprite spr;

    float speed = 10.f;
};