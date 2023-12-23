#pragma once
#include "PhysicsComponent.hpp"
#include "VoxelManager.hpp"

class CollisionManager {
    public:

        struct CollisionState {
            bool hasCollision = false;
            bool isLiquidContact = false;
        };

        // Returns true if collisions found
        static CollisionState handleCollisionsWith(IntPhysicsComponent& player_physicscomp, VoxelManager& main_world, const sf::Vector2u &hitBox) {
            CollisionState result;
            // ちゅき！ いいえ、これわ　悪いです！

            sf::Vector2f ground = player_physicscomp.transform_position;

            std::pair<int, sf::Vector2f> groundCollision = main_world.getPixelCollision(sf::Vector2f(player_physicscomp.transform_position.x + hitBox.x / 2, player_physicscomp.transform_position.y + hitBox.y));

                // ごわみずです！
                
            if(groundCollision.first == 5) {
                result.isLiquidContact = true;
                groundCollision.first = 0;
            }

            for(int i = -10; i < 1; i++) {     
                auto temp_ground = sf::Vector2f(player_physicscomp.transform_position.x,
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

            //if(result.isLiquidContact) return result;

            const std::pair<int, sf::Vector2f> headCollision = main_world.getPixelCollision(sf::Vector2f(player_physicscomp.transform_position.x,
                                                                player_physicscomp.transform_position.y)); 

            const std::pair<int, sf::Vector2f> rightCollision = main_world.getPixelCollision(sf::Vector2f(player_physicscomp.transform_position.x + hitBox.x,
                                                                player_physicscomp.transform_position.y + hitBox.y / 2)); 

            const std::pair<int, sf::Vector2f> leftCollision = main_world.getPixelCollision(sf::Vector2f(player_physicscomp.transform_position.x,
                                                                player_physicscomp.transform_position.y + hitBox.y / 2));

            const bool sideCollision = rightCollision.first || leftCollision.first;

            if(sideCollision && result.isLiquidContact == false) {
                player_physicscomp.transform_position.x -= player_physicscomp.velocity.x;
            }

            if(groundCollision.first == 1) {
                player_physicscomp.transform_position.y = ground.y - PlayerGlobal::characterHitBoxSize.y;
                player_physicscomp.velocity_buffer = 0.f;

                result.hasCollision = true;
            }

            if(headCollision.first) {
                //player_physicscomp.velocity.y = 0;
            }

            return result;
        }

        // Returns true if collisions found
        static CollisionState handleCollisionsWith(PhysicsComponent& player_physicscomp, VoxelManager& main_world, const sf::Vector2u &hitBox) {
            CollisionState result;
            // ちゅき！ いいえ、これわ　悪いです！

            sf::Vector2f ground = player_physicscomp.transform_position;

            std::pair<int, sf::Vector2f> groundCollision = main_world.getPixelCollision(sf::Vector2f(player_physicscomp.transform_position.x + hitBox.x / 2, player_physicscomp.transform_position.y + hitBox.y));

                // ごわみずです！
                
            if(groundCollision.first == 5) {
                result.isLiquidContact = true;
                groundCollision.first = 0;
            }

            for(int i = -10; i < 1; i++) {     
                auto temp_ground = sf::Vector2f(player_physicscomp.transform_position.x,
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

            //if(result.isLiquidContact) return result;

            const std::pair<int, sf::Vector2f> headCollision = main_world.getPixelCollision(sf::Vector2f(player_physicscomp.transform_position.x,
                                                                player_physicscomp.transform_position.y)); 

            const std::pair<int, sf::Vector2f> rightCollision = main_world.getPixelCollision(sf::Vector2f(player_physicscomp.transform_position.x + hitBox.x,
                                                                player_physicscomp.transform_position.y + hitBox.y / 2)); 

            const std::pair<int, sf::Vector2f> leftCollision = main_world.getPixelCollision(sf::Vector2f(player_physicscomp.transform_position.x,
                                                                player_physicscomp.transform_position.y + hitBox.y / 2));

            const bool sideCollision = rightCollision.first || leftCollision.first;

            if(sideCollision && result.isLiquidContact == false) {
                player_physicscomp.transform_position.x -= player_physicscomp.velocity.x;
            }

            if(groundCollision.first == 1) {
                player_physicscomp.transform_position.y = ground.y - PlayerGlobal::characterHitBoxSize.y;
                player_physicscomp.velocity.y = 0.f;

                result.hasCollision = true;
            }

            if(headCollision.first) {
                //player_physicscomp.velocity.y = 0;
            }

            return result;
        }
};