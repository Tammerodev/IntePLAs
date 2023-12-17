#pragma once

#include <list>
#include <array>
#include <SFML/Graphics/Color.hpp>
#include <future>
#include <thread>

#include "Elements.hpp"
#include "math.hpp"
#include "Chunk.hpp"
#include "FastNoiseLite.hpp"
#include "ChunkIndexerVoxelContainer.hpp"

#include "WorldJsonReader.hpp"

#include "Desert.hpp"
#include "Forest.hpp"


class ProcGenerate {
public:
    bool generate(ChunkIndexer& grid, int64_t world_sx, int64_t world_sy) {

        init(time(0));
        initBiomes();

        generateHeightMap(grid, world_sx, world_sy);
        generateWater(grid, 1100, world_sx, world_sy);
        build(grid, world_sx, world_sy);

        return true;
    }

    const float getHeightOnMap(const int index) {
        return heightMap1D.at(index);
    }

    Biome &getBiomeAtPosition(int x, ChunkIndexer& grid) {
        return *biomes.at(std::clamp(grid.getChunkFromPos(x, 0).x, 0, (int)biomes.size() - 1));
    }

private:

    void build(ChunkIndexer& grid, const int world_sx, const int world_sy) {
        int ind = 0;
        int biome_edge = 0;

        std::shared_ptr<Biome>& biome = biomes.at(math::randIntInRange(0, biomes.size() - 1));

        for(auto h : heightMap1D) {
            
            biome = biomes.at(
                std::clamp(grid.getChunkFromPos(ind, 0).x, 0, (int)biomes.size() - 1)
            );

            for(int i = world_sy - 1; i >= 2048 - h; i--) {
    
                sf::Color col = biome->getColorAt(i, h, world_sy);

                grid.getChunkAt(ind/Chunk::sizeX, i/Chunk::sizeY).image.setPixel(ind%Chunk::sizeX, i%Chunk::sizeY, col);

                grid.getChunkAt(ind/Chunk::sizeX, i/Chunk::sizeY).arr[ind%Chunk::sizeX][i%Chunk::sizeY] = 
                    getValueFromCol(col, sf::Vector2i(ind, i));

            }
            ind++;
        }
    }

    void init(const int seed) {
        fsl.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Perlin);
        fsl.SetFractalOctaves(13);
        fsl.SetFractalGain(10.0);
        fsl.SetFractalLacunarity(50.5);
        
        fsl.SetSeed(seed); // Set a random seed (change this to get different noise patterns)
    }

    void initBiomes() {

        for(int i = 0; i < worldSize::world_sx; i++) {
            std::shared_ptr<Biome> biome = nullptr;

            int biomeType = (int)(fsl.GetNoise((float)i * 1.0f, 0.f) * 100.f);

            if(biomeType < 0) {
                biome = std::make_shared<Desert>();
            } else {
                biome = std::make_shared<Forest>();
            }

            if(biome != nullptr)
                biomes.push_back(biome);
        }
    }

    void generateHeightMap(ChunkIndexer& grid, const int world_sx, const int world_sy) {
        // Generate Simplex noise values
        std::shared_ptr<Biome>& biome = biomes.at(math::randIntInRange(0, biomes.size() - 1));

        for (int x = 0; x <= world_sx - 1; x++) {

            biome = biomes.at(
                std::clamp(grid.getChunkFromPos(x, 0).x, 0, (int)biomes.size() - 1)
            );

            float y = biome->getNoise(fsl, (float)x); // Get 1D Simplex noise value for x
            heightMap1D.push_back(y);
        }
    }

    void generateWater(ChunkIndexer& grid, const int waterLevel, const int world_sx, const int world_sy) {
        for(int y = world_sy; y > waterLevel; y--) {
            for(int x = 0; x < world_sx; x++) {
                grid.boundSetImagePixelAt(x, y, elm::Water);
                grid.boundGetVoxelAt(x, y).value = elm::ValWater;
            }
        }
    }
private:

    FastNoiseLite fsl;
    std::vector<float> heightMap1D;
    bool generationDone;

    std::vector<std::shared_ptr<Biome>> biomes; 
    

private:
    
};