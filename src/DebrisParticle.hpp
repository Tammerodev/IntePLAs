#pragma once
#include "Particle.hpp"
#include "math.hpp"
#include "PhysicsComponent.hpp"

class Debris : public Particle {
    public:
        Debris() {
            x = 0.f;
            y = 0.f;
        }

        Debris(const sf::Vector2f& pos, const sf::Vector2f &vel, const sf::Color &col) {
            physComponent.transform_position = pos;
            this->x = pos.x;
            this->y = pos.y;


            physComponent.velocity = vel;

            rect.setSize(sf::Vector2f(1.f, 1.f));
            rect.setFillColor(col);

            rect.setRotation(math::randIntInRange(0, 360));
        }

        void update(const float dt) {
            timer++;

            rect.setPosition(physComponent.transform_position);
            
            physComponent.update(1.0f);

            // Slow down
            physComponent.velocity /= 1.003f;

            rect.setRotation(
                physComponent.transform_rotation
                );

            this->x = physComponent.transform_position.x;
            this->y = physComponent.transform_position.y;

            int a = rect.getFillColor().a - 1;
            if(a <= 0) a = 0;

            rect.setFillColor(
                sf::Color(
                    rect.getFillColor().r,
                    rect.getFillColor().g,
                    rect.getFillColor().b,
                    a
                )
            );

        }

        void render(sf::RenderTarget& target) {
            target.draw(rect);
        }

        bool remove() {
            return timer > lifetime;
        }

        void collide() {
            physComponent.velocity = -physComponent.velocity;
            physComponent.velocity /= 1.5f;

            energy--;
        }

        ParticleType getType() {
            return ParticleType::Debris;
        }

    private:

        sf::RectangleShape rect;
        PhysicsComponent physComponent;

        unsigned int timer = 0;
        unsigned int lifetime = 255;
};