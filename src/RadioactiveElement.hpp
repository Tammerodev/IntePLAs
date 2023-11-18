#pragma once
#include <SFML/Graphics/Vertex.hpp>

class RadioactiveElement : public sf::Vector2i {
    public:
        virtual void update(ChunkIndexer &) = 0;
        virtual int getRadiationStrength() = 0;
        virtual bool clear() = 0;
        
    private:
};