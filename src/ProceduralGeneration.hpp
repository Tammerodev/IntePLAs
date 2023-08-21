#pragma once

#include <list>
#include <array>
#include <SFML/Graphics/Color.hpp>

#include "Elements.hpp"
#include "math.hpp"
#include "Chunk.hpp"

class ProcGenerate {
public:
    bool generate(ChunkIndexer& grid, int64_t world_sx, int64_t world_sy) {
        std::array<sf::Color, 6> colr {
            sf::Color(50, 168, 82),
            elm::Carbon,
            elm::Carbon,
            elm::Lead,
            elm::Titanium,
            elm::Aluminium
        };

        const float val = math::randFloat() * 3;

        for(int x = 0; x <= world_sx - 10; x++ ) {
            const float fx = x / 400.0;
            heightMap1D.push_back(abs(1000+((sin(2*fx) + sin(val * fx)) * 50.0)));
        }
        
        int ind = 0;
        for(auto h : heightMap1D) {
            for(int i = world_sy - 1; i >= 2048 - h; i--) {
                int offset = (rand() % 100 + 100);
                int colorIndex = std::clamp(((i - (int)h) - offset) / 200, 0, (int)colr.size() - 1);
                sf::Color col = colr.at(colorIndex);

                grid.getChunkAt(ind/Chunk::sizeX, i/Chunk::sizeY).image.setPixel(ind%Chunk::sizeX, i%Chunk::sizeY, col);

                grid.getChunkAt(ind/Chunk::sizeX, i/Chunk::sizeY).arr[ind%Chunk::sizeX][i%Chunk::sizeY] = 
                    getValueFromCol(col, sf::Vector2i(ind, i));
            }
            ind++;
        }
        return true;
    }

    
private:
    std::list<float> heightMap1D;
    bool generationDone;
};