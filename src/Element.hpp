#pragma once
#include <SFML/Graphics.hpp>
#include "Chunk.hpp"
#include "ChunkIndexerVoxelContainer.hpp"

class Element : public sf::Vector2i {
    public:
        Element() {
            start_pos = *this;
        }

        virtual void update(ChunkIndexer&) = 0;

        virtual bool clear() {return false;}

        virtual void setVelocity(const sf::Vector2i& pos) {
            
        } 

        virtual std::shared_ptr<Element> turn_into() {
            return nullptr;
        }

        virtual sf::Vector2i move_this_to_chunk(ChunkIndexer& world) {
            sf::Vector2i chunk = world.getChunkFromPos(x, y);

            return chunk;
        }

    private:
        sf::Vector2i start_pos;
};