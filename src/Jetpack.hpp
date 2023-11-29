#pragma once
#include <memory>
#include <list>
#include "ExplosiveBullet.hpp"
#include "Item.hpp"
#include "VoxelManager.hpp"
#include "common.hpp"
#include "SoundFX.hpp"
#include "World.hpp"

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
		const bool use = sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);

		gun_spr.setPosition(pos.x + 9, pos.y + 13);

        if(use) {
            player.getPhysicsComponent().velocity_buffer -= abs(sin(clock.getElapsedTime().asSeconds())) * 0.3;

            if(player.getPhysicsComponent().velocity_buffer < -5.5) {
                player.getPhysicsComponent().velocity_buffer = -5.5;
            }

            for(int i = 0; i < (int)-(player.getPhysicsComponent().velocity_buffer - 10); i++) {
                sf::Color particleColor = sf::Color::Red;
                particleColor.r += math::randIntInRange(-80, 0);
                particleColor.g += math::randIntInRange(0, 80);
                particleColor.b += math::randIntInRange(0, 80);

                sf::Vector2f vel = sf::Vector2f(
                    math::randFloat() * player.getPhysicsComponent().velocity_buffer, math::randFloat() * 2.0
                );

                if(math::randIntInRange(0,1) == 0) {
                    vel.x = -vel.x;
                }

                world.main_world.launchDebrisParticle(sf::Vector2i(gun_spr.getPosition()), particleColor, vel, math::randIntInRange(2, 4));
            }
        }

		gun_spr.setTextureRect(
			sf::IntRect(
				sf::Vector2i(0, use * 16),
				sf::Vector2i(20, 16)
			)
		);
    }

private:
    sf::Clock clock;
	
	sf::Sprite gun_spr;
	sf::Texture gun_tx;

};