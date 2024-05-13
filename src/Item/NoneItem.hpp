#pragma once 
#include "Gun.hpp"

class NoneItem : public Item {
public:
	NoneItem()  {

	}

    void use(Player& player, const sf::Vector2f& mouse, World &world) {

    }

    void render(sf::RenderTarget &target) {
		
    }

	void inventory_render(sf::RenderTarget&r, const sf::Vector2f &pos) {
		

	}

    void setPosition(const sf::Vector2f&p) {
		
	}

	sf::Sprite &getSprite() {
        return sprite;
	}

    void update(World &world, const sf::Vector2f& mousePos, const sf::Vector2f& pos, const float dt, Player&) {
		
    }

    Category getCategory() {
        return Category::Undefined;
    }
private:
    sf::Sprite sprite;
};