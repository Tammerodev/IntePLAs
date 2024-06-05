#pragma once
#include "../debug_globals.hpp"
#include "../common.hpp"
#include "../ChunkIndexerVoxelContainer.hpp"
#include "LightGlobal.hpp"
#include "../ShaderEffects.hpp"

class LightSimulation {
    public:
        void load();

        void update(ChunkIndexer&, ShaderEffect&);

        void render(sf::RenderTarget&);
};