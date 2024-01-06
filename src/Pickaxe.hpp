#pragma once
#include <memory>
#include <list>
#include "ExplosiveBullet.hpp"
#include "Item.hpp"
#include "VoxelManager.hpp"
#include "common.hpp"
#include "SoundFX.hpp"
#include "World.hpp"

class Pickaxe : public Item {
public:
	Pickaxe(VoxelManager &vx_manager, const float _rotation_speed, const int _strength) 
        : rotation_speed(_rotation_speed), strength(_strength)
    {
		gun_tx.loadFromFile("res/img/Tool/Pickaxe.png");
		gun_spr.setTexture(gun_tx);

		gun_spr.setOrigin(gun_spr.getGlobalBounds().width / 2, 16);

        state = PickaxeState::Idle;
	}

    void use(const sf::Vector2f& playerpos,const sf::Vector2f& mouse, World &world) {
        if((mouse.x - playerpos.x) < 0)
            state = PickaxeState::Swing_Right;
        if((mouse.x - playerpos.x) > 0)
            state = PickaxeState::Swing_Left;

		SFX::pickaxe_swing.play();
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

    void update(World &world, const sf::Vector2f& mousePos, const sf::Vector2f& pos, const float dt, Player&) {
		gun_spr.setPosition(pos.x + 9, pos.y + 13);
		gun_spr.setRotation(rotation);

        if(state == PickaxeState::Idle)
            rotation = 0.f;
        if(state == PickaxeState::Swing_Left) {
            rotation += rotation_speed;

            if(rotation >= 180.f)
                state = PickaxeState::Idle;

            const sf::Vector2f check_position = gun_spr.getTransform().transformPoint(16, 16);
            const auto collision = world.main_world.getChunkIndexer().getPixelCollision(check_position);

            if(collision.first) {
                state = PickaxeState::Idle;

                world.main_world.mine(sf::Vector2i(check_position), strength, gains);
            }
        }

        if(state == PickaxeState::Swing_Right) {
            rotation -= rotation_speed;

            if(rotation <= -180.f)
                state = PickaxeState::Idle;

            const sf::Vector2f check_position = gun_spr.getTransform().transformPoint(16, 16);
            const auto collision = world.main_world.getChunkIndexer().getPixelCollision(check_position);

            if(collision.first) {
                state = PickaxeState::Idle;

                world.main_world.mine(sf::Vector2i(check_position), strength, gains);
            }
        }

        gun_spr.setRotation(rotation);
        
    }
private:
	sf::Sprite gun_spr;
	sf::Texture gun_tx;
	uint64_t explosion_stength = 5;
	sf::Texture bullet_tx;

    float rotation = 0.f;
    float rotation_speed = 0.f;
    int strength = 0;

    int gains = 75;

    enum PickaxeState {
        Idle, Swing_Right, Swing_Left, Hit
    } state;
};