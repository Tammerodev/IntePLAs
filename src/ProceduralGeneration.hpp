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

#include "Biome/Desert.hpp"
#include "Biome/Forest.hpp"
#include "Biome/Ocean.hpp"
#include "Biome/Mountains.hpp"
#include <stdint.h>
#include "Settings.hpp"
#include "AddVoxelGroups.hpp"

namespace GenerationGlobals {
    inline int high_ = 0;
    inline int low_ = 0;
}

namespace BiomeUtils {
    static std::shared_ptr<Biome> getBiomeFromName(const std::string& name) {
        if(name == "Desert")        return std::make_shared<Desert>();
        if(name == "Forest")        return std::make_shared<Forest>();
        if(name == "Ocean")         return std::make_shared<Ocean>();
        if(name == "Mountains")     return std::make_shared<Mountains>();

        return nullptr;
    }
}


class ProcGenerate {
private:
    unsigned int water_level = 1100;

public:
    bool generate(ChunkIndexer& grid, int64_t world_sx, int64_t world_sy) {
        // This is bad but its cool
        const int seed = (rand() % math::randIntInRange(0, 100) + time(0)) - (intptr_t)&world_sx;
        init(seed);
        
        loginf("World generation seed = ", seed, ".");

        initBiomes(worldSize::world_sx);

        generateHeightMap(grid, world_sx, world_sy);
        generateWater(grid, water_level, world_sx, world_sy);
        build(grid, world_sx, world_sy);

        generateCaves(grid, world_sx, world_sy);

        for(int i = 0; i < 25; i++) {
            spawnHouse(grid, world_sx, world_sy);
        }

        return true;
    }



    void loadBiomeData(const std::string path) {
        if(!biomes.empty()) {
            prnerr("Biome data is not empty!", "");
            return;
        }

        std::fstream bd_file;
        bd_file.open(StorageSettings::save_path + path + "/data/biomeData.dat");

        std::string line = "";

        while(!bd_file.eof()) {
            std::getline(bd_file, line);

            if(line.empty()) continue;
            
            std::shared_ptr<Biome> biome = BiomeUtils::getBiomeFromName(line);

            if(biome != nullptr)
                biomes.emplace_back(biome);
            else 
                prnerr("Biome was nullptr, input data name was : ", line);
        }

        bd_file.close();
    }

    void loadHeightMap(ChunkIndexer& grid, const std::string path) {
        if(!heightMap1D.empty()) {
            prnerr("Height map not empty!", "");
            return;
        }

        std::fstream hm_file;
        hm_file.open(StorageSettings::save_path + path + "/data/heightMap.dat");

        if(!hm_file.is_open()) {
            prnerr("World save does not contain height data", "!");
            initBiomes(chunks_x);
            generateHeightMap(grid, 0, worldSize::world_sx);

            hm_file.close();
            return;
        }

        std::string line = "";

        while(!hm_file.eof()) {
            std::getline(hm_file, line);

            float value = 0.f;

            if(!line.empty())
                value = std::stod(line);
            else 
                prndd("Invalid value!");

            heightMap1D.emplace_back(value);
        }

        hm_file.close();
    }


    // NOTE : Do not save for a long time! Vector location will invalidate
    std::vector<std::shared_ptr<Biome>>& getBiomeData() {
        return biomes;
    }

    void clear() {
        heightMap1D.clear();
        biomes.clear();
    }

    const float getHeightOnMap(int index) {
        if(heightMap1D.empty()) prnerr("Height map is empty!", "");
        if(index >= heightMap1D.size() - 1) index = heightMap1D.size() - 1;

        return heightMap1D.at(index);
    }

    Biome &getBiomeAtPosition(int x, ChunkIndexer& grid) {
        return *biomes.at(std::clamp(grid.getChunkFromPos(x, 0).x, 0, (int)biomes.size() - 1));
    }

public:
    bool isAreaOccupied(ChunkIndexer& grid, sf::Vector2u size, sf::Vector2i position) {
        for(uint16_t y = 0; y < size.y - 1; y++) {
            for(uint16_t x = 0; x < size.x - 1; x++) {
                
                sf::Vector2i pos = position + sf::Vector2i(x, y);
                grid.boundVector(pos);

                if(grid.getVoxelAt(pos.x, pos.y).value == VoxelValues::WATER ||
                  grid.getImagePixelAt(pos.x, pos.y) == elm::getInfoFromType(VoxelValues::WOOD).color) {
                    return true;
                }

            }
        }

        return false;
    }

private:

    void generateCaves(ChunkIndexer& grid, const int world_sx, const int world_sy) {
        fsl.SetNoiseType(WorldSettings::noiseType);
        fsl.SetFractalOctaves(WorldSettings::noiseOctaves);
        fsl.SetFrequency(WorldSettings::noiseFrequency);

        fsl.SetDomainWarpType(FastNoiseLite::DomainWarpType::DomainWarpType_BasicGrid);
        fsl.SetDomainWarpAmp(0.5);
        fsl.SetFractalGain(1.0);

        float a = 0.f;

        for(uint16_t y = 0; y < world_sy; y++) {
            for(uint16_t x = 0; x < world_sx; x++) {

                const std::string& currentBiomeName = getBiomeAtPosition(x, grid).getName();
                if(currentBiomeName == "Desert") continue;

                if(grid.boundGetVoxelAt(x, y).value == VoxelValues::SAND || grid.boundGetVoxelAt(x, y).value == VoxelValues::WATER) continue;

                float treshold = WorldSettings::noiseTreshold - (y / (world_sy * 3)) - (a / 10.0);

                float noiseValue = fsl.GetNoise((float)x, (float)y);

                if(noiseValue > treshold) {
                    if(currentBiomeName == "Ocean") {
                        // We wouldnt want empty caves in oceans, would we?
                        if(y > water_level) {
                            grid.boundGetVoxelAt(x, y).value = VoxelValues::WATER;
                            grid.boundSetImagePixelAt(x, y, elm::getInfoFromType(VoxelValues::WATER).color);
                        }
                    } else {
                        grid.boundClearVoxelAt(x, y);
                    }
                }
            }
        }
    }

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
        fsl.SetSeed(seed); // Set a random seed (change this to get different noise patterns)

        fsl.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Perlin);
        fsl.SetFractalOctaves(13);
        fsl.SetFractalGain(10.0 + seed);
        fsl.SetFractalLacunarity(50.5);
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
  
    void spawnHouse(ChunkIndexer& grid, const int world_sx, const int world_sy) {
        
        sf::Vector2i position;

        position.x = math::randIntInRange(0, world_sx - 1);
        position.y = getHeightOnMap(position.x);

        if(getBiomeAtPosition(position.x, grid).getName() == "Forest" || getBiomeAtPosition(position.x, grid).getName() == "Desert" ) {
            sf::Image image;
            image.loadFromFile("res/img/Procedural/Buildings/Houses/forest_house.png");

            sf::Vector2u size = image.getSize();

            if(isAreaOccupied(grid, size, position - sf::Vector2i(0, size.y))) return;                      

            for(uint16_t y = 0; y < size.y - 1; y++) {
                for(uint16_t x = 0; x < size.x - 1; x++) {
                    sf::Vector2i pos = position + sf::Vector2i(x, y);
                    pos.y -= size.y;

                    grid.boundSetImagePixelAt(pos.x, pos.y, image.getPixel(x, y));
                }
            }

            // Add itemcreator

            VoxelGroupPropeties propeties;
            propeties.info = "Um";  // (U)i, (m)oulding

            sf::Texture tx;
            tx.loadFromFile("res/img/Tool/moulder.png");

            auto item = std::make_shared<VoxelGroup>();
            item->load(tx.copyToImage());
            item->setPosition(sf::Vector2f(position.x + 30, position.y - 10));
            item->setPropeties(propeties);

            AddVoxelGroups::add_more_worlds.emplace_back(item);
        }
    }


private:

    FastNoiseLite fsl;
    std::vector<float> heightMap1D;
    bool generationDone;

    std::vector<std::shared_ptr<Biome>> biomes; 
    

private:
    
};