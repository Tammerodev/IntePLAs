#pragma once
#include <memory>
#include <list>
#include "Item.hpp"
#include "VoxelManager.hpp"
#include "common.hpp"
#include "World.hpp"

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

    void update(World &world, const sf::Vector2f &pos, const sf::Vector2f& mspos, const float dt) {
		mospos = sf::Vector2i(pos);
		mospos /= 16;
		mospos *= 16;

		// Set position and rotation
		gun_spr.setPosition(mospos.x + gun_spr.getGlobalBounds().width / 2, mospos.y + gun_spr.getGlobalBounds().height / 2);

		sf::Vector2i testPosition = sf::Vector2i(gun_spr.getPosition());
		allowedToplace = !vx_man.getPixelCollision(testPosition);

		if(allowedToplace)
			gun_spr.setColor(sf::Color(255,255,255,200));
		else
			gun_spr.setColor(sf::Color(255,25,25,255));

		if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			gun_spr.rotate(-1);
			gun_spr.setPosition(pos.x, pos.y);	
		}
		else {
			gun_spr.setRotation(0);
		}
    }

	void use(const sf::Vector2f& player,const sf::Vector2f& mouse, World&world) {
		if(!allowedToplace) return;
		if(sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
			vx_man.build_image(sf::Vector2i(mouse), copy_, &world.add_worlds, gun_spr.getRotation(), 3.0f);
		} else {
			vx_man.build_image(sf::Vector2i(mospos), copy_, nullptr);
		}
	}
private:

	sf::Vector2i mospos;


	bool allowedToplace = true;
	bool launchLoaded = false;

	sf::Sprite gun_spr;
	sf::Texture gun_tx;
	
	sf::Image copy_;
	VoxelManager& vx_man;

};