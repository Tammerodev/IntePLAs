#pragma once
#include "Element.hpp"
#include "math.hpp"
#include "ChunkIndexerVoxelContainer.hpp"

class GravityElement : public Element {
    public:
        GravityElement(int x, int y) : Element() {
            this->x = x;
            this->y = y;    
            first = true;
        }

        virtual void run_rules(ChunkIndexer& world, sf::Vector2i& nextWaterPos) {};
        virtual void custom_update(ChunkIndexer& world, sf::Vector2i& nextWaterPos) {};
        virtual void move_(sf::Vector2i& nextWaterPos);

        void update(ChunkIndexer& world);

        bool clear() {
            return remove;
        }
        
    protected:
        sf::Vector2i velocity {0, 0};
        int terminal_velocity = 4;

        void setVoxelInWorld(ChunkIndexer& world);
        void clearLastPos(const sf::Vector2i& nextWaterPos, ChunkIndexer& world);
        void checkExisting(ChunkIndexer& world);
        void setPosition(sf::Vector2i& nextWaterPos);

        bool remove = false;

        sf::Color color {};
        uint8_t value = 0;

        bool first = true;
};