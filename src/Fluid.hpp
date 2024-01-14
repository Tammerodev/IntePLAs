#pragma once
#include "Element.hpp"
#include "math.hpp"
#include "ChunkIndexerVoxelContainer.hpp"

class Fluid : public Element {
    public:
        Fluid(int x, int y) : Element() {
            this->x = x;
            this->y = y;    
        }

        virtual void fluidUpdate(ChunkIndexer& world) = 0;

        void update(ChunkIndexer& world);

        bool clear() {
            return remove;
        }
        
    protected:
        sf::Vector2i velocity {0, 0};

        void move(sf::Vector2i& nextWaterPos);

        void setVoxelInWorld(ChunkIndexer& world);
        void clearLastPos(const sf::Vector2i& nextWaterPos, ChunkIndexer& world);
        void checkExisting(ChunkIndexer& world);

        bool remove = false;

        sf::Color color;
        uint8_t value;
};