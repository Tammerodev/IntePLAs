#pragma once
#include "Biome.hpp"

class Desert : public Biome {
public:

    const std::string getName() {
        return "Desert";
    }

    Desert() : Biome() {

        frequency = 0.05f;
        amplitude = 350.f;

        colorLayers = {
            std::array<sf::Color, 6> {
                elm::Sand,
                elm::Sand,
                elm::Sand,          
                elm::Sand,        
                elm::Sand,
                elm::Sand
            },
            
            std::array<sf::Color, 6> {
                elm::Carbon,
                elm::Carbon,
                elm::Lead,
                elm::Sand,
                elm::Sand
            },

            std::array<sf::Color, 6> {
                elm::Carbon,
                elm::Carbon,
                elm::Lead,
                elm::Sand,
                elm::Sand
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
    }
};