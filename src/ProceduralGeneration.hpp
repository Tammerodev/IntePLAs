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
#include "Ocean.hpp"
#include "Mountains.hpp"

namespace GenerationGlobals {
    inline int high_ = 0;
    inline int low_ = 0;
}


class ProcGenerate {
public:
    bool generate(ChunkIndexer& grid, int64_t world_sx, int64_t world_sy) {

        init(time(0));
        initBiomes(worldSize::world_sx);

        generateHeightMap(grid, world_sx, world_sy);
        generateWater(grid, 1100, world_sx, world_sy);
        build(grid, world_sx, world_sy);

        for(int i = 0; i < 100; i++) {
            spawnHouse(grid, world_sx, world_sy);
        }

        return true;
    }

    void clear() {
        heightMap1D.clear();
        biomes.clear();
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

                grid.getChunkAt(ind/Chunk::sizeX, i/Chunk::sizeY).getImage().setPixel(ind%Chunk::sizeX, i%Chunk::sizeY, col);

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

    void initBiomes(int biome_count) {

        for(int i = 0; i < biome_count; i++) {
            std::shared_ptr<Biome> biome = nullptr;

            int biomeType = (int)(fsl.GetNoise((float)i * 1.0f, 0.f) * 100.f);

            // Rather complex rules...
            if(biomeType > 10) {
                biome = std::make_shared<Mountains>();
            } else if(biomeType < 0 && biomeType > -10) {   // Wait... why not if(biomeType == 0)?
                biome = std::make_shared<Desert>();
            } else if(biomeType < -10) {
                biome = std::make_shared<Ocean>();
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

        
        float amplitude = 300.f;
        float freq = 5.0f;
        float elevation = 1000.f;

        for (int x = 0; x <= world_sx - 1; x++) {

            biome = biomes.at(
                std::clamp(grid.getChunkFromPos(x, 0).x, 0, (int)biomes.size() - 1)
            );

            amplitude += ((biome->getAmplitude() - amplitude) / 100.0f);
            elevation += ((biome->getElevation() - elevation) / 100.0f);

            float y = biome->getNoise(fsl, (float)x / 100.f, amplitude, freq, elevation); // Get 1D Simplex noise value for x

            if(y < GenerationGlobals::low_) 
                GenerationGlobals::low_ = y;

            if(y > GenerationGlobals::high_) 
                GenerationGlobals::high_ = y;

            heightMap1D.push_back(y);
        }
    }

    void generateWater(ChunkIndexer& grid, const int waterLevel, const int world_sx, const int world_sy, const int start = 0) {
        for(int y = world_sy; y > waterLevel; y--) {
            for(int x = 0; x < world_sx; x++) {
                grid.boundSetImagePixelAt(x + start, y, elm::getInfoFromType(VoxelValues::WATER).color);
                grid.boundGetVoxelAt(x + start, y).value = VoxelValues::WATER;
            }
        }
    }
  
    void spawnHouse(ChunkIndexer& grid,  const int world_sx, const int world_sy) {
        
        sf::Vector2i position;

        position.x = math::randIntInRange(0, world_sx - 1);
        position.y = 2048 - getHeightOnMap(position.x);

        if(getBiomeAtPosition(position.x, grid).getName() == "Forest") {
            sf::Image image;
            image.loadFromFile("res/img/Procedural/Buildings/Houses/forest_house.png");

            sf::Vector2u size = image.getSize();

            for(uint16_t y = 0; y < size.y - 1; y++) {
                for(uint16_t x = 0; x < size.x - 1; x++) {

                    sf::Vector2i pos = position + sf::Vector2i(x, y);
                    pos.y -= size.y;

                    grid.boundSetImagePixelAt(pos.x, pos.y, image.getPixel(x, y));
                }
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