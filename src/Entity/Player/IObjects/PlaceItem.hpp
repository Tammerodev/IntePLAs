#pragma once
#include <memory>
#include <list>
#include "Item.hpp"
#include "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/src/VoxelWorld/VoxelManager.hpp"
#include "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/src/common.hpp"

class PlaceItem : public Item {
    public:
    PlaceItem(VoxelManager &vx_manager,const std::string&tx_path, const std::string&copyimg) : vx_man(vx_manager) {

		copy_.loadFromFile(copyimg);
		gun_tx.loadFromFile(tx_path);
		gun_spr.setTexture(gun_tx);

		gun_spr.setOrigin(gun_spr.getGlobalBounds().width / 2,gun_spr.getGlobalBounds().height / 2);

		pl_thread = std::thread(place_task,std::ref(vx_manager), std::ref(positions), std::ref(copy_));

	}

	virtual ~PlaceItem() {
        if (pl_thread.joinable()) {
            pl_thread.join();
		}
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
		vx_man.copy();
		positions.push_back(sf::Vector2i(mouse));
	}
private:
	std::thread pl_thread;
	std::list<sf::Vector2i> positions;

	sf::Sprite gun_spr;
	sf::Texture gun_tx;
	
	sf::Image copy_;
	VoxelManager& vx_man;
	static void place_task(VoxelManager& vx_manager, std::list<sf::Vector2i> &pos, sf::Image &ifmg) {
		while(true) {
			bool use = false;
			sf::Clock timer;
			for(auto &p : pos) {
				vx_manager.lock();
				vx_manager.build_image(p,ifmg);
				use = true;
			}
			if(use) {
				vx_manager.release();
				pos.clear();
			}
		}
	}

};