#pragma once
#include "../../Item/Gun.hpp"

class HelicopterTurret {
    public:

        void load() {
            bullet_tx.loadFromFile("res/img/Tool/bullet.png");
        }

        void use(Player& player, sf::Vector2f launch_pos, const sf::Vector2f& mouse, World &world) {
            auto ex = std::make_unique<ExplosiveBullet>(bullet_tx, launch_pos);
            
            ex->setRotation(10);

            const sf::Vector2f velocity = math::normalize(math::subVector(player.getHeadPosition(), launch_pos)) * 0.5f;

            ex->setVelocity(velocity);
            bullets.push_back(std::move(ex));

            int sound = math::randIntInRange(0, 2);
            
            if(sound == 0) SFX::rocket_launcher_fire0.play();
            if(sound == 1) SFX::rocket_launcher_fire1.play();
            if(sound == 2) SFX::rocket_launcher_fire2.play();

        }

        void render(sf::RenderTarget &target) {
            for(const auto& b : bullets) {
                b->draw(target);
            }
        }
    
        void update(const float dt, World& world) {
            // Update bullets
            for(const auto& bullet : bullets) {
                if(world.main_world.getChunkIndexer().getPixelCollision(bullet->pos).first == ChunkIndexer::CollisionType::Fluid) {
                    bullet->vel -= bullet->vel / 15.f;
                }	
                bullet->update(dt);
            }

            // Check for collisions
            bullets.remove_if([&](const std::unique_ptr<ExplosiveBullet>& bullet) {
                const long MAX_DISTANCE_FROM_MOUSE = 5000;
                
                if (world.main_world.getChunkIndexer().doesLineContainMaterial(sf::Vector2i(bullet->prev_pos), sf::Vector2i(bullet->pos)) ||
                    (world.checkEntityCollisions(bullet->pos) && bullet->timer > 15)) {
                    world.main_world.holeRayCast(sf::Vector2i(bullet->pos), explosion_strength, true, temp);
                    return true; // Remove the bullet
                }

                for(auto &wr : world.add_worlds) {
                    // TODO other worlds...
                    sf::FloatRect bullet_physComp = bullet->getCollider();
                    if(wr->getCollider().intersects(bullet_physComp)) {
                        wr->hole(sf::Vector2i(wr->getPosition() - sf::Vector2f(bullet_physComp.left, bullet_physComp.top)), explosion_strength, true, temp);
                    }
                }

                // Check if bullet is too far...
                if(math::distance(bullet->pos, Controls::worldCursorPos) > MAX_DISTANCE_FROM_MOUSE) {
                    // Delete
                    return true;
                }
                
                return false; // Keep the bullet
            });
        }

    private:
    	std::list<std::unique_ptr<ExplosiveBullet>> bullets;
        int explosion_strength = 100;
        int temp = 0;

        sf::Texture bullet_tx;

};



class HelicopterMachineGun {
    public:

        void load() {
            bullet_tx.loadFromFile("res/img/Tool/gun_bullet.png");
        }

        void use(Player& player, sf::Vector2f launch_pos, const sf::Vector2f& mouse, World &world) {
            auto ex = std::make_unique<ExplosiveBullet>(bullet_tx, launch_pos);
            
            ex->setRotation(10);

            const sf::Vector2f velocity = math::normalize(math::subVector(player.getHeadPosition(), launch_pos)) * 2.5f;

            ex->setVelocity(velocity);
            bullets.push_back(std::move(ex));

            SFX::gun_shot.play();

        }

        void render(sf::RenderTarget &target) {
            for(const auto& b : bullets) {
                b->draw(target);
            }
        }
    
        void update(const float dt, World& world) {
            // Update bullets
            for(const auto& bullet : bullets) {
                if(world.main_world.getChunkIndexer().getPixelCollision(bullet->pos).first == ChunkIndexer::CollisionType::Fluid) {
                    bullet->vel -= bullet->vel / 15.f;
                }	
                bullet->update(dt);
            }

            // Check for collisions
            bullets.remove_if([&](const std::unique_ptr<ExplosiveBullet>& bullet) {
                const long MAX_DISTANCE_FROM_MOUSE = 5000;
                
                if (world.main_world.getChunkIndexer().doesLineContainMaterial(sf::Vector2i(bullet->prev_pos), sf::Vector2i(bullet->pos)) ||
                    (world.damageEntityWithCollision(bullet->pos, explosion_strength) && bullet->timer > 15)) {

                    return true; // Remove the bullet
                }

                for(auto &wr : world.add_worlds) {
                    // TODO other worlds...
                    sf::FloatRect bullet_physComp = bullet->getCollider();
                    if(wr->getCollider().intersects(bullet_physComp)) {
                        wr->hole(sf::Vector2i(wr->getPosition() - sf::Vector2f(bullet_physComp.left, bullet_physComp.top)), explosion_strength, true, temp);
                    }
                }

                // Check if bullet is too far...
                if(math::distance(bullet->pos, Controls::worldCursorPos) > MAX_DISTANCE_FROM_MOUSE) {
                    // Delete
                    return true;
                }
                
                return false; // Keep the bullet
            });
        }

    private:
    	std::list<std::unique_ptr<ExplosiveBullet>> bullets;
        int explosion_strength = 1;
        int temp = 0;

        sf::Texture bullet_tx;

};