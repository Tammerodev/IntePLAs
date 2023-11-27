#pragma once
#include <SFML/Graphics.hpp>
#include "Chunk.hpp"

class Element : public sf::Vector2i {
    public:
        Element() {
            start_pos = *this;
        }

        virtual void update(ChunkIndexer&) = 0;

        virtual bool clear() {return false;};

        virtual sf::Vector2i move_this_to_chunk(ChunkIndexer& world) {
            sf::Vector2i chunk = world.getChunkFromPos(x, y);

            return chunk;
        }

    private:
        sf::Vector2i start_pos;
};