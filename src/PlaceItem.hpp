#pragma once
#include <memory>
#include <list>
#include "Item.hpp"
#include "VoxelManager.hpp"
#include "common.hpp"

class PlaceItem : public Item {
    public:
    PlaceItem(VoxelManager &vx_manager,const std::string&tx_path, const std::string& copyimg) : vx_man(vx_manager) {

		copy_.loadFromFile(copyimg);
		gun_tx.loadFromFile(tx_path);
		gun_spr.setTexture(gun_tx);

		gun_spr.setOrigin(gun_spr.getGlobalBounds().width / 2,gun_spr.getGlobalBounds().height / 2);
	}

    void render(sf::RenderTarget &target) {
		target.draw(gun_spr);
    }

	void setPosition(const sf::Vector2f&p) {
		gun_spr.setPosition(p);
	}

	sf::Sprite &getSprite() {
		return gun_spr;
	}

    void update(VoxelManager &vx_manager, const sf::Vector2f &pos, const sf::Vector2f& mospos, const float dt) {
		// Set position and rotation
		gun_spr.setPosition(pos.x, pos.y);
    }

	void use(const sf::Vector2f& player,const sf::Vector2f& mouse) {
		vx_man.build_image(sf::Vector2i(mouse), copy_);
	}
private:

	sf::Sprite gun_spr;
	sf::Texture gun_tx;
	
	sf::Image copy_;
	VoxelManager& vx_man;

};