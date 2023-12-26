#pragma once
#include <memory>
#include <list>
#include "Item.hpp"
#include "VoxelManager.hpp"
#include "common.hpp"
#include "World.hpp"
#include "ThrowInteface.hpp"

class PlaceItem : public Item {
    public:
    PlaceItem(VoxelManager &vx_manager,const std::string&tx_path, const std::string& copyimg) : vx_man(vx_manager) {

		copy_.loadFromFile(copyimg);
		gun_tx.loadFromFile(copyimg);
		gun_spr.setTexture(gun_tx);

		gun_spr.setOrigin(gun_spr.getGlobalBounds().width / 2,gun_spr.getGlobalBounds().height / 2);
	}

    void render(sf::RenderTarget &target) {
		target.draw(gun_spr);

		throwInterface.render(target);
    }

	void inventory_render(sf::RenderTarget&r, const sf::Vector2f &pos) {
		const sf::Vector2f original_position = gun_spr.getPosition();
		const float original_rotation = gun_spr.getRotation();

		gun_spr.setPosition(pos + sf::Vector2f(gun_spr.getGlobalBounds().width / 2, gun_spr.getGlobalBounds().height / 2));
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

    void update(World &world, const sf::Vector2f &pos, const sf::Vector2f& mspos, const float dt, Player& player) {
		mospos = sf::Vector2i(pos);
		mospos /= 16;
		mospos *= 16;

		// Set position and rotation
		gun_spr.setPosition(mospos.x + gun_spr.getGlobalBounds().width / 2, mospos.y + gun_spr.getGlobalBounds().height / 2);

		sf::Vector2i testPosition = sf::Vector2i(gun_spr.getPosition());
		
		allowedToplace = !vx_man.getPixelCollision(sf::Vector2f(testPosition)).first;
		allowedToplace = true;
		
		if(allowedToplace)
			gun_spr.setColor(sf::Color(255,255,255,200));
		else
			gun_spr.setColor(sf::Color(255,25,25,255));

		if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			throwInterface.enter();
		}
		else {
			throwInterface.exit();
		}

		throwInterface.update(player.getPhysicsComponent().transform_position, pos);
    }

	void use(const sf::Vector2f& player,const sf::Vector2f& mouse, World&world) {
		if(!allowedToplace) return;
		if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			vx_man.build_image(sf::Vector2i(throwInterface.getThrowPosition()), copy_, &world.add_worlds, throwInterface.getThrowVelocity() / 50.f);
		} else {
			vx_man.build_image(sf::Vector2i(mospos), copy_, nullptr, sf::Vector2f(0.f, 0.f), true);
		}
	}
private:

	sf::Vector2i mospos;

	ThrowInteface throwInterface;

	bool allowedToplace = true;
	bool launchLoaded = false;

	sf::Sprite gun_spr;
	sf::Texture gun_tx;
	
	sf::Image copy_;
	VoxelManager& vx_man;

};