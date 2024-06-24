#pragma once
#include "Particle.hpp"
#include "../math.hpp"

class FreeNeutron : public Particle {
    public:
        FreeNeutron() {
            x = 0.f;
            y = 0.f;
        }

        FreeNeutron(const sf::Vector2f& pos, const sf::Vector2f &vel) : velocity(vel * 5.0f) {
            x = pos.x;
            y = pos.y;

            rect.setSize(sf::Vector2f(1.f, 1.f));
            rect.setFillColor(sf::Color(255, 100, 100));
            rect.setOrigin(0.5f, 0.5f);
        }

        void update(const float dt, const sf::Vector2f &pos) {
            timer++;

            rect.setPosition(*this);
            
            *this += velocity;

            rect.rotate(math::randFloat());
            rect.rotate(-math::randFloat());
        }

        void render(sf::RenderTarget& target) {
            target.draw(rect);
        }

        bool remove() {
            return timer > lifetime;
        }

        void collide() {
            velocity = -velocity;
            *this += velocity;

            energy--;
        }

        ParticleType getType() {
            return ParticleType::Neutron;
        }

    private:
        sf::RectangleShape rect;
        unsigned int timer = 0;
        unsigned int lifetime = 1000;

        sf::Vector2f velocity {0.f, 0.f};
};