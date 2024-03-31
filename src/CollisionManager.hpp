#pragma once
#include "PhysicsComponent.hpp"
#include "ChunkIndexerVoxelContainer.hpp"
#include "PlayerGlobal.hpp"

class CollisionManager {
    public:

        struct CollisionState {
            bool hasCollision = false;
            bool isLiquidContact = false;
        };

        static CollisionState playerHandleCollisions(IntPhysicsComponent& player_physicscomp, ChunkIndexer& main_world, const sf::Vector2u &hitBox) {
            CollisionState result;
            sf::Vector2f ground = player_physicscomp.transform_position;
            std::pair<int, sf::Vector2f> groundCollision = main_world.getPixelCollision(sf::Vector2f(player_physicscomp.transform_position.x + hitBox.x / 2, player_physicscomp.transform_position.y + hitBox.y));
  
            if(groundCollision.first == 5) {
                result.isLiquidContact = true;
                groundCollision.first = 0;
            }

            for(int x = -10; x < 10; x++) {     
                for(int i = -10; i < 1; i++) {     
                    auto temp_ground = sf::Vector2f(player_physicscomp.transform_position.x + x,
                                                (player_physicscomp.transform_position.y + PlayerGlobal::characterHitBoxSize.y) + i);

                    auto temp_groundCollision = main_world.getPixelCollision(temp_ground);

                    if(temp_groundCollision.first == 5) {
                        result.isLiquidContact = true;
                        temp_groundCollision.first = 0;
                    } else if(temp_groundCollision.first != 0) {
                        temp_groundCollision.first = 1;
                    }

                    ground = temp_ground;
                    groundCollision = temp_groundCollision;

                    if(groundCollision.first == 1) 
                        break;
                }
            }

            const std::pair<int, sf::Vector2f> headCollision = main_world.getPixelCollision(sf::Vector2f(player_physicscomp.transform_position.x,
                                                                player_physicscomp.transform_position.y)); 

            const std::pair<int, sf::Vector2f> rightCollision = main_world.getPixelCollision(sf::Vector2f(player_physicscomp.transform_position.x + hitBox.x,
                                                                player_physicscomp.transform_position.y + hitBox.y / 1.55)); 

            const std::pair<int, sf::Vector2f> leftCollision = main_world.getPixelCollision(sf::Vector2f(player_physicscomp.transform_position.x,
                                                                player_physicscomp.transform_position.y + hitBox.y / 1.55));

            const bool sideCollision = rightCollision.first || leftCollision.first;

            if(sideCollision && !result.isLiquidContact)
                player_physicscomp.transform_position.x -= player_physicscomp.velocity.x;

            if(groundCollision.first == 1) {
                player_physicscomp.transform_position.y = ground.y - PlayerGlobal::characterHitBoxSize.y;
                player_physicscomp.velocity_buffer = 0.f;
                result.hasCollision = true;
            }

            if(headCollision.first == 1) {
                player_physicscomp.velocity.y = 0;
                player_physicscomp.velocity_buffer = -1.f;
                player_physicscomp.transform_position.y += 1;
                result.hasCollision = true;
            }

            return result;
        }


        static CollisionState handleCollisionsWith(PhysicsComponent& player_physicscomp, ChunkIndexer& main_world, const sf::Vector2u &hitBox) {
            CollisionState result;

            sf::Vector2f ground = player_physicscomp.transform_position;

            std::pair<int, sf::Vector2f> groundCollision = main_world.getPixelCollision(sf::Vector2f(player_physicscomp.transform_position.x + hitBox.x / 2, player_physicscomp.transform_position.y + hitBox.y));
                
            if(groundCollision.first == 5) {
                result.isLiquidContact = true;
                groundCollision.first = 0;
            }

            for(int x = -10; x < 10; x++) {     
                for(int i = -10; i < 1; i++) {     
                    auto temp_ground = sf::Vector2f(player_physicscomp.transform_position.x + x,
                                                (player_physicscomp.transform_position.y + PlayerGlobal::characterHitBoxSize.y) + i);

                    auto temp_groundCollision = main_world.getPixelCollision(temp_ground);

                    if(temp_groundCollision.first == 5) {
                        result.isLiquidContact = true;
                        temp_groundCollision.first = 0;
                    } else if(temp_groundCollision.first != 0) {
                        temp_groundCollision.first = 1;
                    }

                    ground = temp_ground;
                    groundCollision = temp_groundCollision;

                    if(groundCollision.first == 1) 
                        break;
                }
            }

            //const std::pair<int, sf::Vector2f> headCollision = main_world.getPixelCollision(sf::Vector2f(player_physicscomp.transform_position.x,
            //                                                    player_physicscomp.transform_position.y)); 

            const std::pair<int, sf::Vector2f> rightCollision = main_world.getPixelCollision(sf::Vector2f(player_physicscomp.transform_position.x + hitBox.x,
                                                                player_physicscomp.transform_position.y + hitBox.y / 2)); 

            const std::pair<int, sf::Vector2f> leftCollision = main_world.getPixelCollision(sf::Vector2f(player_physicscomp.transform_position.x,
                                                                player_physicscomp.transform_position.y + hitBox.y / 2));

            const bool sideCollision = rightCollision.first || leftCollision.first;

            if(sideCollision && result.isLiquidContact == false)
                player_physicscomp.transform_position.x -= player_physicscomp.velocity.x;

            if(groundCollision.first == 1) {
                player_physicscomp.transform_position.y = ground.y - PlayerGlobal::characterHitBoxSize.y;
                player_physicscomp.velocity.y = 0.f;
                result.hasCollision = true;
            }
            
            return result;
        }
};