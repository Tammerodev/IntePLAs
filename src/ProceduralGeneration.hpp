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


class ProcGenerate {
public:
    bool generate(ChunkIndexer& grid, int64_t world_sx, int64_t world_sy) {

        init(time(0));
        generateHeightMap(world_sx, world_sy);
        generateWater(grid, 1100, world_sx, world_sy);
        build(grid, world_sx, world_sy);

        return true;
    }

    const float getHeightOnMap(const int index) {
        return heightMap1D.at(index);
    }


private:

    void build(ChunkIndexer& grid, const int world_sx, const int world_sy) {
        int ind = 0;
        for(auto h : heightMap1D) {
            for(int i = world_sy - 1; i >= 2048 - h; i--) {
                int offset = math::randIntInRange(0, 100) + 100;

                int colorLayer = std::clamp
                    (
                    (int)((i + h + offset) / (world_sy / 6)) - 1, 0, (int)colorLayers.size() - 1
                    );


                sf::Color col = colorLayers.at(colorLayer).at(math::randIntInRange(0, colorLayers.size() - 1));

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

    void generateHeightMap(const int world_sx, const int world_sy) {
        // Generate Simplex noise values
        for (int x = 0; x <= world_sx - 1; x++) {
            float y = (fsl.GetNoise((float)x / 10.0f, 0.f) * 300.0) + 1000.0; // Get 1D Simplex noise value for x
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

private:
    std::array<std::array<sf::Color, 6>, 6> colorLayers {
        std::array<sf::Color, 6> {
            sf::Color(69, 169, 65),
            sf::Color(69, 169, 69),
            sf::Color(61, 160, 65),            
            sf::Color(68, 168, 69),            
            sf::Color(59, 169, 70),
            sf::Color(65, 149, 70)
        },
        
        std::array<sf::Color, 6> {
            elm::Carbon,
            elm::Carbon,
            elm::Lead,
            sf::Color(96,96,96), 
            sf::Color(96,96,96)
        },

        std::array<sf::Color, 6> {
            elm::Carbon,
            elm::Carbon,
            elm::Lead,
            sf::Color(96,96,96), 
            sf::Color(96,96,96)
        },

        std::array<sf::Color, 6> {
            elm::Carbon,
            elm::Carbon,
            elm::Lead,
            elm::Titanium,
            elm::Aluminium
        },

        std::array<sf::Color, 6> {
            elm::Lead,
            elm::Carbon,
            elm::Lead,
            elm::Titanium,
            elm::Aluminium
        },

        std::array<sf::Color, 6> {
            elm::Carbon,
            elm::Carbon,
            elm::Lead,
            elm::Titanium,
            elm::Aluminium
        }
    };
};