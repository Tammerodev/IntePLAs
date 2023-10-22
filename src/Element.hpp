#pragma once
#include <SFML/Graphics.hpp>
#include "Chunk.hpp"

class Element : public sf::Vector2i {
    public:
        virtual void update(ChunkIndexer&) = 0;

    private:
};