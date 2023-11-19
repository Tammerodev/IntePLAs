#pragma once
#include "Chunk.hpp"
#include "VoxelContainer.hpp"

class ChunkIndexer : public VoxelContainer {
public:
    ChunkIndexer() {
        update();
    }

    void init() {
        for(int y = 0; y < chunks_y; y++) {
            std::vector<Chunk> v;
            for(int x = 0; x < chunks_x; x++) {
                v.push_back(Chunk());
            }
        
            gridPos.push_back(v);
        }
    }

    void update() {
        world_sx = Chunk::sizeX * chunks_x;
        world_sy = Chunk::sizeY * chunks_y;

        world_snegx = Chunk::sizeX * chunks_negx;
        world_snegy = Chunk::sizeY * chunks_negy;
    }

    Chunk& getChunkAt(const int x, const int y) {
        if(x >= 0) {
           return gridPos.at(abs(y)).at(x);         
        }
        return gridNeg.at(abs(y)).at(x);
    }

    void boundVector(sf::Vector2i &v) {
        if(v.y < 0) v.y = 0;
        if(v.x < -chunks_negx) v.x = 0;
        if(v.y > world_sy - 1) v.y = world_sy - 1;
        if(v.x > world_sx - 1) v.x = world_sx - 1;
    }

    const sf::Vector2i getBoundedVector(const sf::Vector2i &p) {
        sf::Vector2i v = p;

        if(v.y < 0) v.y = 0;
        if(v.x < -chunks_negx) v.x = 0;
        if(v.y > world_sy - 1) v.y = world_sy - 1;
        if(v.x > world_sx - 1) v.x = world_sx - 1;

        return v;
    }

    const sf::Vector2i getChunkFromPos(const int x, const int y) {
        return sf::Vector2i(x/Chunk::sizeX, y/Chunk::sizeY);
    }

    Voxel &getVoxelAt (const int x, const int y) {
        return getChunkAt(x/Chunk::sizeX, y/Chunk::sizeY).arr[abs(x%Chunk::sizeX)][abs(y%Chunk::sizeY)];
    }
    
    Voxel &boundGetVoxelAt (const int x, const int y) {
        sf::Vector2i pos = sf::Vector2i(x, y);

        boundVector(pos);

        return getChunkAt(pos.x/Chunk::sizeX, pos.y/Chunk::sizeY).arr[abs(pos.x%Chunk::sizeX)][abs(pos.y%Chunk::sizeY)];
    }


    void boundSetImagePixelAt(const int x, const int y, const sf::Color& color) {
        sf::Vector2i pos = sf::Vector2i(x, y);
        boundVector(pos);

        getChunkAt(getChunkFromPos(pos.x, pos.y).x, getChunkFromPos(pos.x, pos.y).y).modified = true;

        getChunkAt(pos.x/Chunk::sizeX, pos.y/Chunk::sizeY).image.setPixel(pos.x%Chunk::sizeX, pos.y%Chunk::sizeY, color);
    }

    const sf::Color getImagePixelAt(const int x, const int y) {

        return getChunkAt(x/Chunk::sizeX, y/Chunk::sizeY).image.getPixel(x%Chunk::sizeX, y%Chunk::sizeY);
    }

    void clearVoxelAt(const int x, const int y) {
        getVoxelAt(x,y).value = 0; 
        setImagePixelAt(x,y,sf::Color(0,0,0,0));
    }
    
    void damageVoxelAt(const int x, const int y) {
        uint8_t &strenght = getVoxelAt(x,y).strenght;
        --strenght;
        if(getVoxelAt(x,y).strenght <= 0) { 
            clearVoxelAt(x,y);
        }
    }

    void setImagePixelAt(const int x, const int y, const sf::Color& color) {
        getChunkAt(getChunkFromPos(x, y).x, getChunkFromPos(x, y).y).modified = true;

        getChunkAt(x/Chunk::sizeX, y/Chunk::sizeY).image.setPixel(x%Chunk::sizeX, y%Chunk::sizeY, color);
    }

    void SetHeat(const uint64_t x, const uint64_t y, int64_t temp)
    {
        Voxel &vox = getVoxelAt(x, y);

        if(vox.temp >= elm::getMaxTempFromType(vox.value)) {
            uint8_t &strenght = vox.strenght;
            --strenght;
            if(vox.strenght <= 0) { 
                getVoxelAt(x,y).value = 0; 
                setImagePixelAt(x,y,sf::Color(0,0,0,0));
            }
        }

        vox.temp = temp;

        if(vox.temp <= 0) vox.temp = 0;

        sf::Color currPixel = getImagePixelAt(x,y);

        uint64_t valR = vox.temp * 1; 
        if(valR >= 255) valR = 255;
        currPixel.r = valR;
        if(valR > 300) {
            currPixel.g = 255;
            currPixel.b = 255;
        }

        setImagePixelAt(x,y,currPixel);
    }

    void heatVoxelAt(const int x, const int y, const int temp)
    {
        Voxel &vox = getVoxelAt(x, y);
        getChunkAt(getChunkFromPos(x, y).x, getChunkFromPos(x, y).y).modified = true;

        if(vox.temp >= elm::getMaxTempFromType(vox.value)) {
            uint8_t &strenght = vox.strenght;
            --strenght;
            if(vox.strenght <= 0) { 
                getVoxelAt(x,y).value = 0; 
                setImagePixelAt(x,y,sf::Color(0,0,0,0));
            }
        }

        vox.temp += temp;

        if(vox.temp <= 0) vox.temp = 0;

        sf::Color currPixel = getImagePixelAt(x,y);

        uint64_t valR = vox.temp * 1; 
        if(valR >= 255) valR = 255;
        currPixel.r = valR;
        if(valR > 300) {
            currPixel.g = 255;
            currPixel.b = 255;
        }

        setImagePixelAt(x,y,currPixel);
    }


    int64_t world_sx;
    int64_t world_sy;

    int64_t world_snegx;
    int64_t world_snegy;

private:
    std::vector<std::vector<Chunk>> gridPos;
    std::array<std::array<Chunk, chunks_negx + 1>, chunks_negy + 1> gridNeg;

};
