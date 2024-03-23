#pragma once
#include <array>
#include "Elements.hpp"
#include "math.hpp"
#include <algorithm>
#include "ChunkIndexerVoxelContainer.hpp"
#include "VegetationInfo.hpp"

class Biome {
    public:
    
    public:
        Biome() {

        }

        const sf::Color getColorAt(int i, int h, int world_sy) {
            int offset = math::randIntInRange(0, 100) + 100;

            int colorLayer = std::clamp
                (
                (int)((i + h + offset) / (world_sy / 6)) - 1, 0, (int)colorLayers.size() - 1
                );


            const auto &layer = colorLayers.at(colorLayer);

            const int randomValue = math::randIntInRange(0, layer.size() - 1);

            const auto &color = layer.at(randomValue);

            sf::Color col = color;
            return col;
        }

        virtual const std::string getName() = 0;

        virtual const BiomeInfo* getInfo() {
            return &info;
        }

        float getNoise(FastNoiseLite &fsl, const float x, float amp, float freq, float elev) {
            return (fsl.GetNoise(x * freq, 0.f) * amp) + elev;
        }

        float getAmplitude() {
            return amplitude;
        }

        float getFrequency() {
            return frequency;
        }

        float getElevation() {
            return elevation;
        }

    protected:
        BiomeInfo info;

        std::array<std::vector<sf::Color>, 6> colorLayers {
            std::vector<sf::Color> {
                sf::Color(69, 169, 65),
                sf::Color(69, 169, 69),
                sf::Color(61, 160, 65),            
                sf::Color(68, 168, 69),            
                sf::Color(59, 169, 70),
                sf::Color(65, 149, 70)
            },
            
            std::vector<sf::Color> {
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::SILICON).color,
                sf::Color(96,96,96), 
                sf::Color(96,96,96)
            },

            std::vector<sf::Color> {
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::SILICON).color,
                sf::Color(96,96,96), 
                sf::Color(96,96,96)
            },

            std::vector<sf::Color> {
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::SILICON).color,
                elm::getInfoFromType(VoxelValues::TITANIUM).color,
                elm::getInfoFromType(VoxelValues::ALUMINIUM).color
            },

            std::vector<sf::Color> {
                elm::getInfoFromType(VoxelValues::SILICON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::SILICON).color,
                elm::getInfoFromType(VoxelValues::TITANIUM).color,
                elm::getInfoFromType(VoxelValues::ALUMINIUM).color
            },

            std::vector<sf::Color> {
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::CARBON).color,
                elm::getInfoFromType(VoxelValues::SILICON).color,
                elm::getInfoFromType(VoxelValues::TITANIUM).color,
                elm::getInfoFromType(VoxelValues::ALUMINIUM).color
            }
        };

        float amplitude = 300.f;
        float frequency = 0.1f;
        float elevation = 1000.f;
};