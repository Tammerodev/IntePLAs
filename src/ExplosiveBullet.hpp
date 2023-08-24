#pragma once
#include <SFML/Graphics.hpp>

class ExplosiveBullet {
public:
    ExplosiveBullet(const sf::Texture &bullet_tx, const sf::Vector2f &pos) {
        setPosition(pos);
        spr.setTexture(bullet_tx);
    }

    virtual ~ExplosiveBullet() {
        
    }

    void update(const float dt) {
        hitbox.left = pos.x;
        hitbox.top = pos.y;
        hitbox.width = 8;
        hitbox.height = 8;

        pos += vel * dt;
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

    sf::FloatRect &getHitbox () {
        return hitbox;
    }

    void setRotation(const float& angle) {
        spr.setRotation(angle);
    }

    sf::Vector2f pos = sf::Vector2f(0,0);
    
private:
    sf::Vector2f vel = sf::Vector2f(0,0);
    sf::FloatRect hitbox;

    sf::Sprite spr;

    float speed = 1.f;

};