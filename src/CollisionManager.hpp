#pragma once
#include "PhysicsComponent.hpp"
#include "VoxelManager.hpp"

class CollisionManager {
    public:
        // Returns true if collisions found
        static bool handleCollisionsWith(IntPhysicsComponent& player_physicscomp, VoxelManager& main_world, const sf::Vector2u &hitBox) {
            bool result = false;

            sf::Vector2f ground = player_physicscomp.transform_position;

            std::pair<bool, sf::Vector2f> groundCollision = main_world.getPixelCollision(sf::Vector2f(player_physicscomp.transform_position.x + hitBox.x / 2, player_physicscomp.transform_position.y + hitBox.y));
                                                                
            for(int i = -10; i < 1; i++) {     
                ground = sf::Vector2f(player_physicscomp.transform_position.x,
                                            (player_physicscomp.transform_position.y + PlayerGlobal::characterHitBoxSize.y) + i);

                groundCollision = main_world.getPixelCollision(ground);

                if(groundCollision.first) 
                    break;
            }

            const std::pair<bool, sf::Vector2f> headCollision = main_world.getPixelCollision(sf::Vector2f(player_physicscomp.transform_position.x,
                                                                player_physicscomp.transform_position.y)); 

            const std::pair<bool, sf::Vector2f> rightCollision = main_world.getPixelCollision(sf::Vector2f(player_physicscomp.transform_position.x + hitBox.x,
                                                                player_physicscomp.transform_position.y + hitBox.y / 2)); 

            const std::pair<bool, sf::Vector2f> leftCollision = main_world.getPixelCollision(sf::Vector2f(player_physicscomp.transform_position.x,
                                                                player_physicscomp.transform_position.y + hitBox.y / 2));

            const bool sideCollision = rightCollision.first || leftCollision.first;

            if(sideCollision) {
                player_physicscomp.transform_position.x -= player_physicscomp.velocity.x;
            }

            if(groundCollision.first) {
                player_physicscomp.transform_position.y = ground.y - PlayerGlobal::characterHitBoxSize.y;

                result = true;
            }

            if(headCollision.first) {
                player_physicscomp.velocity.y = 0;
                player_physicscomp.velocity_buffer = 0;
            }

            return result;
        }

        // Returns true if collisions found
        static bool handleCollisionsWith(PhysicsComponent& player_physicscomp, VoxelManager& main_world, const sf::Vector2i &hitBox) {
            bool result = false;

            sf::Vector2f ground = player_physicscomp.transform_position;

            std::pair<bool, sf::Vector2f> groundCollision = main_world.getPixelCollision(sf::Vector2f(player_physicscomp.transform_position.x + hitBox.x / 2, player_physicscomp.transform_position.y + hitBox.y));
                                                                
            for(int i = -10; i < 1; i++) {     
                ground = sf::Vector2f(player_physicscomp.transform_position.x,
                                            (player_physicscomp.transform_position.y + PlayerGlobal::characterHitBoxSize.y) + i);

                groundCollision = main_world.getPixelCollision(ground);

                if(groundCollision.first) 
                    break;
            }

            const std::pair<bool, sf::Vector2f> headCollision = main_world.getPixelCollision(sf::Vector2f(player_physicscomp.transform_position.x,
                                                                player_physicscomp.transform_position.y)); 

            const std::pair<bool, sf::Vector2f> rightCollision = main_world.getPixelCollision(sf::Vector2f(player_physicscomp.transform_position.x + hitBox.x,
                                                                player_physicscomp.transform_position.y + hitBox.y / 2)); 

            const std::pair<bool, sf::Vector2f> leftCollision = main_world.getPixelCollision(sf::Vector2f(player_physicscomp.transform_position.x,
                                                                player_physicscomp.transform_position.y + hitBox.y / 2));

            const bool sideCollision = rightCollision.first || leftCollision.first;

            if(sideCollision) {
                player_physicscomp.transform_position.x -= player_physicscomp.velocity.x;
            }

            if(groundCollision.first) {
                player_physicscomp.transform_position.y = ground.y - PlayerGlobal::characterHitBoxSize.y;

                result = true;
            }

            if(headCollision.first) {
                player_physicscomp.velocity.y = 0;
            }

            return result;
        }
};