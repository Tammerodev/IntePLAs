#pragma once
#include <list>
#include <SFML/Graphics.hpp>
#include "VoxelManager.hpp"

class VoxelSpy {
    public:
        void alertOfChunkModification(const sf::Vector2i& chunk_pos) {
            chunks_modified.push_back(chunk_pos);
            
        }

        void receiveChunkChanges(ChunkIndexer &world) {

        }

    private:
        std::list<sf::Vector2i> chunks_modified;
};