#pragma once
#include <memory>
#include <list>
#include "ExplosiveBullet.hpp"
#include "Item.hpp"
#include "VoxelManager.hpp"
#include "common.hpp"
#include "SoundFX.hpp"
#include "World.hpp"

class Backpack : public Item {
public:
	Backpack(VoxelManager &vx_manager) {

	}

    void use(const sf::Vector2f& playerpos,const sf::Vector2f& mouse, World &world) {

        for(int i = 0; i < 10; i++) {
            sf::Vector2i pos = sf::Vector2i(playerpos);
            pos.x += math::randIntInRange(-5, 5);
            pos.y += math::randIntInRange(-5, 5);

            prndd("GAS");

            world.main_world.getChunkIndexer().boundVector(pos);

            world.main_world.getChunkIndexer().getVoxelAt(pos.x, pos.y).value = VoxelValues::BUTANE;
            world.main_world.getChunkIndexer().setImagePixelAt(pos.x, pos.y, elm::getInfoFromType(VoxelValues::BUTANE).color);

            world.main_world.addElement(pos.x, pos.y, std::make_shared<Butane>(pos.x, pos.y));
        }
    }

    void render(sf::RenderTarget &target) {

    }

	void inventory_render(sf::RenderTarget&r, const sf::Vector2f &pos) {

	}

    void setPosition(const sf::Vector2f&p) {

	}

	sf::Sprite &getSprite() {
        return sprite;
	}

    void update(World &world, const sf::Vector2f& mousePos, const sf::Vector2f& pos, const float dt, Player& player) {

    }

	Category getCategory() {
		return Category::Tool;
	}

private:
    sf::Sprite sprite;
};