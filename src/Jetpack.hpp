#pragma once
#include "World.hpp"

#include <memory>
#include <list>
#include "ExplosiveBullet.hpp"
#include "Item.hpp"
#include "VoxelManager.hpp"
#include "common.hpp"
#include "SoundFX.hpp"
#include "FireParticle.hpp"

class Jetpack : public Item {
public:
	Jetpack(VoxelManager &vx_manager) {
		gun_tx.loadFromFile("res/img/Tool/Jetpack.png");
		gun_spr.setTexture(gun_tx);

		gun_spr.setOrigin(20 / 2, 16 / 2);

		gun_spr.setTextureRect(
			sf::IntRect(
				sf::Vector2i(0, 0),
				sf::Vector2i(20, 16)
			)
		);

        SFX::rocket.setLoop(true);

	}

    void use(const sf::Vector2f& playerpos,const sf::Vector2f& mouse, World &world) {
        
    }

    void render(sf::RenderTarget &target) {

		target.draw(gun_spr);
    }

	void inventory_render(sf::RenderTarget&r, const sf::Vector2f &pos) {
		sf::IntRect(
				sf::Vector2i(0, 0),
				sf::Vector2i(20, 16)
			);

		const sf::Vector2f original_position = gun_spr.getPosition();
		const float original_rotation = gun_spr.getRotation();

		gun_spr.setPosition(pos);
		gun_spr.setRotation(90);

		r.draw(gun_spr);

		gun_spr.setPosition(original_position);
		gun_spr.setRotation(original_rotation);

	}

    void setPosition(const sf::Vector2f&p) {
		gun_spr.setPosition(p);
	}

	sf::Sprite &getSprite() {
		return gun_spr;
	}

    void update(World &world, const sf::Vector2f& mousePos, const sf::Vector2f& pos, const float dt, Player& player);

	Category getCategory() {
		return Category::Tool;
	}

private:
    sf::Clock clock;
	
	sf::Sprite gun_spr;
	sf::Texture gun_tx;

	bool isEnabled = true;
	int thrust = 0;
};