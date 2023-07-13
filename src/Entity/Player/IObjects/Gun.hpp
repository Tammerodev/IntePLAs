#pragma once
#include <memory>
#include <list>
#include "ExplosiveBullet.hpp"
#include "Item.hpp"
#include "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/src/VoxelWorld/VoxelManager.hpp"
#include "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/src/common.hpp"

class Gun : public Item {
public:
	Gun(VoxelManager &vx_manager,const std::string&bullet_tx_path, const std::string&gun_tx_path, uint64_t strenth) {
		bullet_tx.loadFromFile(bullet_tx_path);
		gun_tx.loadFromFile(gun_tx_path);
		gun_spr.setTexture(gun_tx);

		explosion_stength = strenth;

		gun_spr.setOrigin(gun_spr.getGlobalBounds().width / 2,gun_spr.getGlobalBounds().height / 2);

		explosion_thread = std::thread(thread_task,std::ref(vx_manager), std::ref(positions), explosion_stength);

	}

	virtual ~Gun() {
        if (explosion_thread.joinable()) {
            explosion_thread.join();
		}
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
			bullet->update(dt);
		}

		// Check for collisions
		bullets.remove_if([&](const std::unique_ptr<ExplosiveBullet>& bullet) {
			const long MAX_DISTANCE_FROM_MOUSE = 10000;
			
			if (vx_manager.checkCollisionsWith(bullet->getHitbox()).first) {
				positions.push_back(sf::Vector2i(bullet->pos.x, bullet->pos.y));
				vx_manager.copy();
				SFX::strong_explosion.play();
				return true; // Remove the bullet
			}
			if(abs(bullet->pos.x - mousePos.x) > MAX_DISTANCE_FROM_MOUSE ||
			 abs(bullet->pos.y - mousePos.y) > MAX_DISTANCE_FROM_MOUSE) return true;
			return false; // Keep the bullet
		});
    }
private:
	float rotationAngle;

	std::thread explosion_thread;
	std::list<sf::Vector2i> positions;
	std::list<std::unique_ptr<ExplosiveBullet>> bullets;

	sf::Sprite gun_spr;
	sf::Texture gun_tx;
	float bullet_speed = 1.0f;
	uint64_t explosion_stength = 35;
	
	sf::Texture bullet_tx;

	static void thread_task(VoxelManager& vx_manager, std::list<sf::Vector2i> &pos, uint64_t strength) {
		while(true) {
			bool use = false;
			sf::Clock timer;
			for(auto &p : pos) {
				vx_manager.lock();
				vx_manager.hole(p,strength);
				use = true;
			}
			if(use) {
				vx_manager.release();
				pos.clear();
			}
		}
	}
};