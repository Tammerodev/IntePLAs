#pragma once
#include "Particle.hpp"
#include "../math.hpp"
#include "../PhysicsComponent.hpp"

class FireEffectParticle : public Particle {
    public:
        FireEffectParticle(const sf::Vector2f& pos, const sf::Vector2f &vel, const sf::Color &col, const float size = 1.f, const int ener = 0) {
            this->x = pos.x;
            this->y = pos.y;
            prev_position = *this;

            velocity = vel;

            rect.setSize(sf::Vector2f(size, size));
            rect.setFillColor(col);

            rect.setRotation(math::randIntInRange(0, 360));
            e = ener;
        }

        void update(const float dt, const sf::Vector2f &pos) {
            prev_position = *this;
            
            rect.setPosition(*this);
            *this += velocity * dt;

            timer++;
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
                    std::clamp(rect.getFillColor().g - 25, 0, 255),
                    std::clamp(rect.getFillColor().b - 25, 0, 255),
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
            return ParticleType::FireEffectParticle;
        }
        
        virtual int getEnergy() { 
            return e;
        }

        virtual sf::Color getColor() {
            return rect.getFillColor();
        }

    private:
        int e = 0;

        sf::RectangleShape rect;
        sf::Vector2f velocity {0.f, 0.f};

        unsigned int timer = 0;
        unsigned int lifetime = 10;

        bool rem = false;
};