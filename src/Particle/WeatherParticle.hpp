#pragma once
#include "Particle.hpp"
#include "../math.hpp"
#include "../PhysicsComponent.hpp"
#include "../WeatherState.hpp"

class WeatherParticle : public Particle {
    public:
        WeatherParticle(const sf::Vector2f& pos, const sf::Vector2f &size, const uint8_t speed, const sf::Color col) {
            this->x = pos.x;
            this->y = pos.y;
            this->speed = speed;

            rect.setSize(size);
            rect.setFillColor(col);
            rect.setPosition(pos);

            rect.setRotation(-(WeatherState::windDir * 2.0));
        }

        void update(const float dt, const sf::Vector2f &pos) {
            x = rect.getPosition().x;
            y = rect.getPosition().y;

            rect.move(WeatherState::windDir, (speed / 60.f) * dt);
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

        virtual sf::Color getColor() {
            return rect.getFillColor();
        }

    private:
        sf::RectangleShape rect;
        uint8_t speed = 127;

        bool rem = false;
};