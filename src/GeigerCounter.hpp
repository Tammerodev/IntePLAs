#pragma once
#include <memory>
#include <list>
#include "ExplosiveBullet.hpp"
#include "Item.hpp"
#include "VoxelManager.hpp"
#include "common.hpp"
#include "SoundFX.hpp"
#include "World.hpp"

class GeigerCounter : public Item {
public:
	GeigerCounter(VoxelManager &vx_manager) {
		gun_tx.loadFromFile("res/img/Tool/geiger_counter.png");
		gun_spr.setTexture(gun_tx);

		gun_spr.setOrigin(20 / 2, 16 / 2);

		gun_spr.setTextureRect(
			sf::IntRect(
				sf::Vector2i(0, 0),
				sf::Vector2i(20, 16)
			)
		);

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

    void update(World &world, const sf::Vector2f& mousePos, const sf::Vector2f& pos, const float dt, Player& player) {
		const float rotationAngle = atan2f(mousePos.y - gun_spr.getPosition().y, mousePos.x - gun_spr.getPosition().x) * 180 / math::PI;
		const bool click = PlayerGlobal::radiation_received > 1;

		gun_spr.setPosition(pos.x + 9, pos.y + 13);
		gun_spr.setRotation(rotationAngle);

		if(click) {
			SFX::geigerCounter.play();
		}

		gun_spr.setTextureRect(
			sf::IntRect(
				sf::Vector2i(0, click * 16),
				sf::Vector2i(20, 16)
			)
		);
    }

	Category getCategory() {
		return Category::Tool;
	}

private:
	
	sf::Sprite gun_spr;
	sf::Texture gun_tx;

};