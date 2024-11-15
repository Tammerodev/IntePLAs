#pragma once
#include <memory>
#include <list>
#include "../ExplosiveBullet.hpp"
#include "Item.hpp"
#include "../VoxelManager.hpp"
#include "../common.hpp"
#include "../SoundFX.hpp"
#include "../World.hpp"

class Laser : public Item {
public:
	Laser(VoxelManager &vx_manager)  {
		gun_tx.loadFromFile("res/img/Tool/laser.png");
		gun_spr.setTexture(gun_tx);

		gun_spr.setOrigin(gun_spr.getGlobalBounds().width / 2,gun_spr.getGlobalBounds().height / 2);

        laserBeam.append(sf::Vertex());
        laserBeam.append(sf::Vertex());

        laserBeam.setPrimitiveType(sf::PrimitiveType::Lines);
	}

    void use(Player& player, const sf::Vector2f& mouse, World &world) {

    }

    void render(sf::RenderTarget &target) {
		target.draw(gun_spr);

        if(_use)
            target.draw(laserBeam);
    }

	void inventory_render(sf::RenderTarget&r, const sf::Vector2f &pos) {
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

    void update(World &world, const sf::Vector2f& mousePos, const sf::Vector2f& pos, const float dt, Player&) {
		// Set position and rotation
		rotationAngle = atan2f(mousePos.y - gun_spr.getPosition().y, mousePos.x - gun_spr.getPosition().x) * 180 / math::PI;

		gun_spr.setPosition(pos.x + 9, pos.y + 13);
		gun_spr.setRotation(rotationAngle);

        _use = Controls::use();

        if(_use) {
            // Raycast
            const sf::Vector2f direction = sf::Vector2f(cos(gun_spr.getRotation() * 3.1415926f / 180.f),sin(gun_spr.getRotation() * 3.1415926f / 180.f));

            const sf::Vector2i startPosition = sf::Vector2i(gun_spr.getPosition());
            const sf::Vector2i endPosition = sf::Vector2i(sf::Vector2f(startPosition) + direction * rayLength);

            world.main_world.singleRayCast(startPosition, endPosition, 100, true, temp, 5, false);

            laserBeam[0].position = sf::Vector2f(startPosition);
            laserBeam[1].position = sf::Vector2f(endPosition);

            laserBeam[0].color = sf::Color(255, 10, 20, 233);
            laserBeam[1].color = sf::Color(255, 10, 20, 233);
        }

    }

	Category getCategory() {
		return Category::Tool;
	}


private:
	float rotationAngle = 0.f;
    bool _use = false;

	sf::Sprite gun_spr;
	sf::Texture gun_tx;

    sf::VertexArray laserBeam;

    const float rayLength = 100.f;
	uint64_t temp = 2000;
};