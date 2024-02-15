#pragma once
#include <SFML/Graphics/Vertex.hpp>
#include "ChunkIndexerVoxelContainer.hpp"
#include "ElectricityGlobal.hpp"
#include "common.hpp"

class component_data {
    public:
        int audio_data = 0;
};

class ElectricComponent : public sf::Vector2i {
    public:
        virtual component_data update(ChunkIndexer &) = 0;
        virtual void render(sf::RenderTarget&) {}

        virtual int getVoltage() = 0;
        virtual void setVoltage(int) = 0;

        virtual bool clear() = 0;
        
    protected:
        std::pair<sf::Vector2i, int>* connected_to = nullptr;
};