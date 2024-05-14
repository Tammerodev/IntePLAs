#pragma once
#include "../ChunkIndexerVoxelContainer.hpp"
#include "Player.hpp"

class PlayerTemparatureManager {
    public:
        void checkTemparature(ChunkIndexer& world, sf::Vector2i playerPosition) {
            PlayerGlobal::temparature = 0;

            for(int x = 0; x < checkRange.x; x++) {
                for(int y = 0; y < checkRange.y; y++) {
                    const sf::Vector2i pos = {(x + playerPosition.x) - checkRange.x / 2, (y + playerPosition.y) + checkRange.y / 2};
                    const int temp = world.boundGetVoxelAt(pos.x, pos.y).temp;
                        
                    const float tempFinal = temp / std::clamp(math::distance(sf::Vector2f(playerPosition), sf::Vector2f(pos)), 1.0f, 10000.f);
                
                    PlayerGlobal::temparature += tempFinal;
                }
            }
        }
    private:
        sf::Vector2i checkRange {25,25};
};