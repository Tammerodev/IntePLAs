#pragma once
#include "ChunkIndexerVoxelContainer.hpp"
#include "Player.hpp"

class PlayerTemparatureManager {
    public:
        void checkTemparature(ChunkIndexer& world, sf::Vector2i playerPosition) {
            PlayerGlobal::temparature = 0;

            for(int x = 0; x < checkRange.x; x++) {
                for(int y = 0; y < checkRange.y; y++) {
                    const int temp = world.boundGetVoxelAt((x + playerPosition.x) - checkRange.x / 2, (y + playerPosition.y) + checkRange.y / 2).temp;
                        
                
                    PlayerGlobal::temparature += (float)temp;
                }
            }
        }
    private:
        sf::Vector2i checkRange {25,25};
};