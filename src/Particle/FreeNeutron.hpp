#pragma once
#include "Particle.hpp"
#include "../math.hpp"
#include "../common.hpp"

inline sf::Texture SPR_neutronSprite;

class FreeNeutron : public Particle {
    public:
        FreeNeutron() {
            x = 0.f;
            y = 0.f;
        }

        FreeNeutron(const sf::Vector2f& pos, const sf::Vector2f &vel) : velocity(vel * 2.0f) {
            x = pos.x;
            y = pos.y;

            rect.setOrigin(0.5f, 0.5f);

            // TODO: make this reliable
            if(SPR_neutronSprite.getSize().x != 9) {
                SPR_neutronSprite.loadFromFile("res/img/Particle/neutron.png");
                prndd("Loaded neutron sprite");
            }

            rect.setTexture(SPR_neutronSprite);
            rect.setScale(1.f / 3.f, 1.f / 3.f);
        }

        void update(const float dt, const sf::Vector2f &pos) {
            rect.setPosition(*this);

            int lifetime = 10;

            rect.setColor(sf::Color(255,255,255, rect.getColor().a - lifetime));

            rem = rect.getColor().a - lifetime <= 0;
            
            *this += velocity;

            rect.rotate(math::randFloat());
            rect.rotate(-math::randFloat());
        }

        void render(sf::RenderTarget& target) {
            target.draw(rect);
        }

        bool remove() {
            return rem;
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
        sf::Sprite rect;
        bool rem = false;
        sf::Vector2f velocity {0.f, 0.f};
};