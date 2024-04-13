#pragma once
#include "../../Entity.hpp"
#include "../../Controls.hpp"
#include "../../math.hpp"

class CollectableBox : public EntityComponents {
    public:
        void load() {
            texture.loadFromFile("res/mod/img/box.png");
            sprite.setTexture(texture);
        }

        bool update(const sf::Vector2f &playerPosition) {
            const float distance = math::distance(sprite.getPosition(), playerPosition);

            if(distance < 25 && Controls::collect()) {
                return true;
            }

            return false;
        }

        void render(sf::RenderTarget& target) {
            target.draw(sprite);
        }


    private:
};