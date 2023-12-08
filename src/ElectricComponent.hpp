#pragma once
#include <SFML/Graphics/Vertex.hpp>
#include "ChunkIndexerVoxelContainer.hpp"

class component_data {
    public:
        int audio_data = 0;
};

class ElectricComponent : public sf::Vector2i {
    public:
        virtual component_data update(ChunkIndexer &) = 0;

        virtual int getVoltage() = 0;
        virtual void setVoltage(int) = 0;

        virtual bool clear() = 0;
        
    private:
};