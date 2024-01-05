#pragma once
#include "Particle.hpp"

class FreeNeutron : public Particle {
    public:
        FreeNeutron() {
            x = 0.f;
            y = 0.f;
        }

        FreeNeutron(const sf::Vector2f& pos, const sf::Vector2f &vel) : velocity(vel) {
            x = pos.x;
            y = pos.y;

            rect.setSize(sf::Vector2f(1.f, 1.f));
            rect.setFillColor(sf::Color::Red);

            rect.setRotation(math::randIntInRange(0, 360));
        }

        void update(const float dt, const sf::Vector2f &pos) {
            timer++;

            rect.setPosition(*this);
            
            x += velocity.x;
            y += velocity.y;

            // Slow down
            velocity /= 1.003f;

            rect.rotate(velocity.x + velocity.y);
        }

        void render(sf::RenderTarget& target) {
            target.draw(rect);
        }

        bool remove() {
            return timer > lifetime;
        }

        void collide() {
            velocity = -velocity;
            velocity /= 1.5f;

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