#pragma once
#include <memory>
#include <list>
#include "ExplosiveBullet.hpp"
#include "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/src/VoxelWorld/VoxelManager.hpp"

class Gun {
public:
	Gun(VoxelManager &vx_manager) {
		bullet_tx.loadFromFile("res/img/Tool/bullet.png");
		gun_tx.loadFromFile("res/img/Tool/rocket_launcher.png");
		gun_spr.setTexture(gun_tx);

		gun_spr.setOrigin(gun_spr.getGlobalBounds().width / 2,gun_spr.getGlobalBounds().height / 2);

		explosion_thread = std::thread(thread_task,std::ref(vx_manager), std::ref(positions));

	}

	virtual ~Gun() {
        if (explosion_thread.joinable()) {
            explosion_thread.join();
		}
	}
    void spawn_bullet(sf::Vector2f playerpos) {
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

    void update(VoxelManager &vx_manager, sf::Vector2f mousePos, sf::Vector2f pos, const float &dt) {
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
			if (vx_manager.checkCollisionsWith(bullet->getHitbox()).first) {
				positions.push_back(sf::Vector2i(bullet->pos.x, bullet->pos.y));
				SFX::amplified_explosion.play();
				return true; // Remove the bullet
			}
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
	
	sf::Texture bullet_tx;

	static void thread_task(VoxelManager& vx_manager, std::list<sf::Vector2i> &pos) {
		while(true) {
			for(auto &p : pos) {
				vx_manager.hole(p,35);
			}
			
			pos.clear();
			vx_manager.merge();
		}
	}
};