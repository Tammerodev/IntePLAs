#pragma once
#include <memory>
#include <list>
#include "ExplosiveBullet.hpp"
#include "Item.hpp"
#include "VoxelManager.hpp"
#include "common.hpp"
#include "SoundFX.hpp"

class Gun : public Item {
public:
	Gun(VoxelManager &vx_manager,const std::string&bullet_tx_path, const std::string&gun_tx_path, uint64_t strenth, bool f, int t, bool spin = false) : temp(t), spn(spin) {
		bullet_tx.loadFromFile(bullet_tx_path);
		gun_tx.loadFromFile(gun_tx_path);
		gun_spr.setTexture(gun_tx);

		explosion_stength = strenth;
		gun_spr.setOrigin(gun_spr.getGlobalBounds().width / 2,gun_spr.getGlobalBounds().height / 2);

	}

    void use(const sf::Vector2f& playerpos,const sf::Vector2f& mouse) {
		auto ex = std::make_unique<ExplosiveBullet>(bullet_tx);
		ex->setPosition(playerpos);
		ex->setRotation(rotationAngle);
		ex->setVelocity(sf::Vector2f(cos(gun_spr.getRotation() * 3.1415926f / 180.f),sin(gun_spr.getRotation() * 3.1415926f / 180.f)));
		bullets.push_back(std::move(ex));
    }

    void render(sf::RenderTarget &target) {
		for(const auto& b : bullets) {
			b->draw(target);
		}
		target.draw(gun_spr);
    }

    void setPosition(const sf::Vector2f&p) {
		gun_spr.setPosition(p);
	}

	sf::Sprite &getSprite() {
		return gun_spr;
	}

    void update(VoxelManager &vx_manager, const sf::Vector2f& mousePos, const sf::Vector2f& pos, const float dt) {
		// Set position and rotation
		rotationAngle = atan2f(mousePos.y - gun_spr.getPosition().y, mousePos.x - gun_spr.getPosition().x) * 180 / math::PI;

		gun_spr.setPosition(pos.x + 9, pos.y + 13);
		gun_spr.setRotation(rotationAngle);

		// Update bullets
		for(const auto& bullet : bullets) {
			if(spn) {
				bullet->setRotation(bullet->pos.x + bullet->pos.y);
			}
			bullet->update(dt);
		}

		// Check for collisions
		bullets.remove_if([&](const std::unique_ptr<ExplosiveBullet>& bullet) {
			const long MAX_DISTANCE_FROM_MOUSE = 10000;
			
			if (vx_manager.getOvelapWithRect(bullet->getHitbox()).first) {
				vx_manager.hole(sf::Vector2i(bullet->pos), explosion_stength, true, temp);
				return true; // Remove the bullet
			}
			if(abs(bullet->pos.x - mousePos.x) > MAX_DISTANCE_FROM_MOUSE ||
			 abs(bullet->pos.y - mousePos.y) > MAX_DISTANCE_FROM_MOUSE) return true;
			return false; // Keep the bullet
		});
    }
private:
	bool spn = false;
	float rotationAngle;
	std::list<std::unique_ptr<ExplosiveBullet>> bullets;

	sf::Sprite gun_spr;
	sf::Texture gun_tx;
	float bullet_speed = 1.0f;
	uint64_t explosion_stength = 35;
	uint64_t temp = 100;
	sf::Texture bullet_tx;
};