#pragma once
#include <memory>
#include <list>
#include "ExplosiveBullet.hpp"
#include "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/src/VoxelWorld/VoxelManager.hpp"
#include "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/src/common.hpp"

class PlaceItem {
    public:
    PlaceItem(VoxelManager &vx_manager,const std::string&tx_path, const std::string&copyimg) {
		copy_.loadFromFile(copyimg);
		gun_tx.loadFromFile(tx_path);
		gun_spr.setTexture(gun_tx);

		gun_spr.setOrigin(gun_spr.getGlobalBounds().width / 2,gun_spr.getGlobalBounds().height / 2);

		pl_thread = std::thread(place_task,std::ref(vx_manager), std::ref(positions));

	}

	virtual ~PlaceItem() {
        if (pl_thread.joinable()) {
            pl_thread.join();
		}
	}

    void render(sf::RenderTarget &target) {
		target.draw(gun_spr);
    }

    void update(VoxelManager &vx_manager, sf::Vector2f mousePos, sf::Vector2f pos, const float &dt) {
		// Set position and rotation
		gun_spr.setPosition(pos.x + 9, pos.y + 13);
    }

	void use(sf::Vector2i pos) {
		positions.push_back(sf::Vector2i(pos));
	}
private:
	std::thread pl_thread;
	std::list<sf::Vector2i> positions;

	sf::Sprite gun_spr;
	sf::Texture gun_tx;
	
	sf::Image copy_;

	static void place_task(VoxelManager& vx_manager, std::list<sf::Vector2i> &pos) {
		while(true) {
			bool use = false;
			sf::Clock timer;
			for(auto &p : pos) {
				vx_manager.lock();
				vx_manager.build_circle(p,10);
				use = true;
			}
			if(use) {
				vx_manager.release();
				pos.clear();
			}
		}
	}

};