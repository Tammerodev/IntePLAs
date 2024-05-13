#pragma once
#include "Particle.hpp"
#include "../math.hpp"
#include "../PhysicsComponent.hpp"

class FireParticle : public Particle {
    public:
        FireParticle(const sf::Vector2f& pos, const sf::Vector2f &vel, const sf::Color &col, const float size = 1.f, bool gravity = true) {
            physComponent.transform_position = pos;
            this->x = pos.x;
            this->y = pos.y;
            prev_position = *this;

            
            if(gravity == false) {
                physComponent.disable_gravity();
            }

            physComponent.velocity = vel;

            rect.setSize(sf::Vector2f(size, size));
            rect.setFillColor(col);


            rect.setRotation(math::randIntInRange(0, 360));

            energy = 2;
        }

        void update(const float dt, const sf::Vector2f &pos) {
            prev_position = *this;

            timer++;

            rect.setPosition(physComponent.transform_position);
            
            physComponent.update(1.0f);

            rect.setRotation(physComponent.transform_rotation);

            this->x = physComponent.transform_position.x;
            this->y = physComponent.transform_position.y;

            int a = rect.getFillColor().a - 1;
            if(a <= 0) {
                a = 0;
                rem = true;
            }

    
            int r = rect.getFillColor().r - 1;
            if(r <= 0) r = 0;

            rect.setFillColor(
                sf::Color(
                    r,
                    rect.getFillColor().g,
                    rect.getFillColor().b,
                    a
                )
            );

            if(energy <= 0) rem = true;

        }

        void render(sf::RenderTarget& target) {
            target.draw(rect);
        }

        bool remove() {
            return rem;
        }

        void collide() {
            rem = true;
        }

        ParticleType getType() {
            return ParticleType::FireParticle;
        }

        virtual sf::Color getColor() {
            return rect.getFillColor();
        }

    private:

        sf::RectangleShape rect;
        PhysicsComponent physComponent;

        unsigned int timer = 0;
        unsigned int lifetime = 10;

        bool rem = false;
};