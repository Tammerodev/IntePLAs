#pragma once

#include <list>
#include <array>
#include <SFML/Graphics/Color.hpp>
#include <future>
#include <thread>

#include "Elements.hpp"
#include "math.hpp"
#include "Chunk.hpp"

class ProcGenerate {
public:
    bool generate(ChunkIndexer& grid, int64_t world_sx, int64_t world_sy) {

        prndd("Started generating map");

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
                elm::Carbon,
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

        prndd("Creating heightmap");

        sf::Clock timer;

        const float val = math::randFloat() * 3;

        for(int x = 0; x <= world_sx - 10; x++ ) {
            const float fx = x / 400.0;
            heightMap1D.push_back(abs(1000+((sin(2*fx) + sin(val * fx)) * 50.0)));
        }

        loginf("Creating height map took : ", timer.restart().asSeconds(), ".");

        prndd("Writing map image");

        //*
        //*     TODO : Make this async
        //* 

        int ind = 0;
        for(auto h : heightMap1D) {
            for(int i = world_sy - 1; i >= 2048 - h; i--) {
                int offset = math::randIntInRange(0, 100) + 100;

                int colorLayer = std::clamp(((i - (int)h) - offset) / 200, 0, (int)colorLayers.size() - 1);


                sf::Color col = colorLayers.at(colorLayer).at(math::randIntInRange(0, colorLayers.size() - 1));

                grid.getChunkAt(ind/Chunk::sizeX, i/Chunk::sizeY).image.setPixel(ind%Chunk::sizeX, i%Chunk::sizeY, col);

                grid.getChunkAt(ind/Chunk::sizeX, i/Chunk::sizeY).arr[ind%Chunk::sizeX][i%Chunk::sizeY] = 
                    getValueFromCol(col, sf::Vector2i(ind, i));
            }
            ind++;
        }

        loginf("Writing map image took : ", timer.restart().asSeconds(), ".");        
        
        return true;
    }


    std::list<float> heightMap1D;
    bool generationDone;
};