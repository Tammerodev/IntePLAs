#pragma once
#include "../debug_globals.hpp"
#include "../common.hpp"
#include "../ChunkIndexerVoxelContainer.hpp"
#include "LightGlobal.hpp"

class LightSimulation {
    public:
        void load();

        void update(ChunkIndexer&);

        void render(sf::RenderTarget&);
};