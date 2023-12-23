#pragma once
#include <array>
#include "Elements.hpp"
#include "math.hpp"
#include <algorithm>
#include "ChunkIndexerVoxelContainer.hpp"
#include "VegetationInfo.hpp"

class Biome {
    public:
        Biome() {

        }

        const sf::Color getColorAt(int i, int h, int world_sy) {
            int offset = math::randIntInRange(0, 100) + 100;

            int colorLayer = std::clamp
                (
                (int)((i + h + offset) / (world_sy / 6)) - 1, 0, (int)colorLayers.size() - 1
                );


            sf::Color col = colorLayers.at(colorLayer).at(math::randIntInRange(0, colorLayers.size() - 1));
            return col;
        }

        virtual const std::string getName() = 0;

        virtual const VegetationInfo getVegetationInfo() {
            VegetationInfo vegetationInfo;
            return vegetationInfo;
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

        float amplitude = 300.f;
        float frequency = 0.1f;
        float elevation = 1000.f;
};