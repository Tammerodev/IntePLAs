#pragma once
#include <SFML/Graphics.hpp>

class ExplosiveBullet {
public:
    ExplosiveBullet(const sf::Texture &bullet_tx, const sf::Vector2f &pos) {
        setPosition(pos);
        spr.setPosition(pos);
        spr.setTexture(bullet_tx);
    
        prev_pos = pos;
    }

    virtual ~ExplosiveBullet() {
        
    }

    void update(const float dt) {
        prev_pos = spr.getPosition();

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
    }

    sf::Vector2f pos = sf::Vector2f(0, 0);
    sf::Vector2f prev_pos = sf::Vector2f(0, 0);

private:
    sf::Vector2f vel = sf::Vector2f(0,0);

    sf::Sprite spr;

    float speed = 10.f;
};