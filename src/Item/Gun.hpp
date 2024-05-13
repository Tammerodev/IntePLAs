#pragma once
#include <memory>
#include <list>
#include "../ExplosiveBullet.hpp"
#include "Item.hpp"
#include "../VoxelManager.hpp"
#include "../common.hpp"
#include "../SoundFX.hpp"
#include "../World.hpp"
#include "../FontManager.hpp"

class Gun : public Item {
public:
	Gun(VoxelManager &vx_manager, const std::string&bullet_tx_path, const std::string&gun_tx_path, const uint64_t strenth, const bool f, const int t, const bool spin = false)  {
		temp = t;
		spn = spin;

		bullet_tx.loadFromFile(bullet_tx_path);
		gun_tx.loadFromFile(gun_tx_path);
		gun_spr.setTexture(gun_tx);

		explosion_stength = strenth;
		gun_spr.setOrigin(gun_spr.getGlobalBounds().width / 2,gun_spr.getGlobalBounds().height / 2);

		itemUseCooldown.setCooldown(1.5f);
	}

    void use(Player& player, const sf::Vector2f& mouse, World &world) {
		if(!itemUseCooldown.tryUse()) return;

		auto ex = std::make_unique<ExplosiveBullet>(bullet_tx, player.getPhysicsComponent().transform_position);
		ex->setPosition(sf::Vector2f(gun_spr.getGlobalBounds().left + 20, gun_spr.getGlobalBounds().top + 4));
		ex->setRotation(rotationAngle);

		const sf::Vector2f velocity = sf::Vector2f(cos(gun_spr.getRotation() * 3.1415926f / 180.f),sin(gun_spr.getRotation() * 3.1415926f / 180.f));

		ex->setVelocity(velocity);
		bullets.push_back(std::move(ex));

		player.getPhysicsComponent().addToVelocity(-(velocity * 2.f));

		int sound = math::randIntInRange(0, 2);
		if(sound == 0) SFX::rocket_launcher_fire0.play();
		if(sound == 1) SFX::rocket_launcher_fire1.play();
		if(sound == 2) SFX::rocket_launcher_fire2.play();

    }

    void render(sf::RenderTarget &target) {
		for(const auto& b : bullets) {
			b->draw(target);
		}
		target.draw(gun_spr);
    }

	void inventory_render(sf::RenderTarget&r, const sf::Vector2f& player_position) {
		const sf::Vector2f original_position = gun_spr.getPosition();
		const float original_rotation = gun_spr.getRotation();

		gun_spr.setPosition(player_position);
		gun_spr.setRotation(90);

		r.draw(gun_spr);

		gun_spr.setPosition(original_position);
		gun_spr.setRotation(original_rotation);

		itemUseCooldown.render(r, player_position, sf::Vector2i(sf::Vector2f(16.f, 16.f) * 1.5f));


		// TODO : debug thingy
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F2) && bullets.size() != 0) 
			debug_view::forced_view_center = bullets.front()->pos;
		else 
			debug_view::forced_view_center = sf::Vector2f(0.f, 0.f);

	}

    void setPosition(const sf::Vector2f&p) {
		gun_spr.setPosition(p);
	}

	sf::Sprite &getSprite() {
		return gun_spr;
	}

    void update(World &world, const sf::Vector2f& mousePos, const sf::Vector2f& pos, const float dt, Player&) {
		itemUseCooldown.update();

		// Set position and rotation
		rotationAngle = atan2f(mousePos.y - gun_spr.getPosition().y, mousePos.x - gun_spr.getPosition().x) * 180 / math::PI;

		gun_spr.setPosition(pos.x + 9, pos.y + 13);
		gun_spr.setRotation(rotationAngle);

		// Update bullets
		for(const auto& bullet : bullets) {
			if(spn) {
				bullet->setRotation(bullet->pos.x + bullet->pos.y);
			}

			if(world.main_world.getChunkIndexer().getPixelCollision(bullet->pos).first == ChunkIndexer::CollisionType::Fluid) {
				bullet->vel -= bullet->vel / 15.f;
			}	
			bullet->update(dt);
		}

		// Check for collisions
		bullets.remove_if([&](const std::unique_ptr<ExplosiveBullet>& bullet) {
			const long MAX_DISTANCE_FROM_MOUSE = 5000;
			
			if (world.main_world.getChunkIndexer().doesLineContainMaterial(sf::Vector2i(bullet->prev_pos), sf::Vector2i(bullet->pos)) ||
				(world.checkEntityCollisions(bullet->pos) && bullet->timer > 15)) {
				world.main_world.holeRayCast(sf::Vector2i(bullet->pos), explosion_stength, true, temp);
				return true; // Remove the bullet
			}

			for(auto &wr : world.add_worlds) {
				// TODO other worlds...
				sf::FloatRect bullet_physComp = bullet->getCollider();
				if(wr.getCollider().intersects(bullet_physComp)) {
					wr.hole(sf::Vector2i(wr.getPosition() - sf::Vector2f(bullet_physComp.left, bullet_physComp.top)), explosion_stength, true, temp);
				}
			}

			// Check if bullet is too far...
			if(math::distance(bullet->pos, pos) > MAX_DISTANCE_FROM_MOUSE) {
				// Delete
				return true;
			}
			
			return false; // Keep the bullet
		});
    }

	Category getCategory() {
		return Category::Pairing;
	}


private:
	ItemUseCooldown itemUseCooldown;

	bool spn = false;
	float rotationAngle = 0.f;
	std::list<std::unique_ptr<ExplosiveBullet>> bullets;

	sf::Sprite gun_spr;
	sf::Texture gun_tx;
	uint64_t explosion_stength = 35;
	uint64_t temp = 100;
	sf::Texture bullet_tx;
};