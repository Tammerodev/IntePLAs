#pragma once 
#include "Gun.hpp"
#include "FireParticle.hpp"

class HeatGun : public Item {
public:
	HeatGun(VoxelManager &vx_manager)  {
		gun_tx.loadFromFile("res/img/Tool/Heatgun.png");
		gun_spr.setTexture(gun_tx);

		gun_spr.setOrigin(gun_spr.getGlobalBounds().width / 2,gun_spr.getGlobalBounds().height / 2);
	}

    void use(const sf::Vector2f& playerpos,const sf::Vector2f& mouse, World &world) {

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
		// Set position and rotation
		rotationAngle = atan2f(mousePos.y - gun_spr.getPosition().y, mousePos.x - gun_spr.getPosition().x) * 180 / math::PI;

		gun_spr.setPosition(pos.x + 9, pos.y + 13);
		gun_spr.setRotation(rotationAngle);

        if(Controls::use()) {
            for(int i = 0; i < 1; i++) {
                sf::Color particleColor = sf::Color::Red;
                particleColor.r += math::randIntInRange(-80, 0);
                particleColor.g += math::randIntInRange(0, 200);
                particleColor.b += math::randIntInRange(0, 100);

                sf::Vector2f vel = sf::Vector2f(
                    cos(gun_spr.getRotation() * 3.1415926f / 180.f),sin(gun_spr.getRotation() * 3.1415926f / 180.f)
                );

                vel *= 5.0f;

                vel.x += math::randFloat() / 10.f;
                vel.y += math::randFloat() / 10.f;

                std::shared_ptr<FireParticle> particle = std::make_shared<FireParticle>(
                    gun_spr.getPosition(),
                    vel,
                    particleColor,
                    math::randIntInRange(2, 4),
                    false
                );

                world.main_world.launchParticle(particle);

            }
        }
    }

    Category getCategory() {
        return Category::Tool;
    }
private:
	float rotationAngle = 0.f;

	sf::Sprite gun_spr;
	sf::Texture gun_tx;
};