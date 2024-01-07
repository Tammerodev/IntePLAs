#pragma once
#include "Chunk.hpp"
#include "VoxelContainer.hpp"
#include "MaterialPack.hpp"

namespace worldSize {
    inline int64_t world_sx;
    inline int64_t world_sy;
}

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

    void expandChunksBy(int x_chunks) {
        for(int y = 0; y < chunks_y; y++) {
            std::vector<Chunk> v;
            for(int x = 0; x < x_chunks; x++) {
                v.push_back(Chunk());
            }
        
            gridPos.push_back(v);
        }
    }

    void update() {
        worldSize::world_sx = Chunk::sizeX * chunks_x;
        worldSize::world_sy = Chunk::sizeY * chunks_y;

        world_snegx = Chunk::sizeX * chunks_negx;
        world_snegy = Chunk::sizeY * chunks_negy;
    }

    Chunk& getChunkAt(const int x, const int y) {
        if(x >= 0) {
           return gridPos.at(abs(y)).at(x);         
        }
        return gridNeg.at(abs(y)).at(x);
    }

    Chunk& boundGetChunkAt(const int x, const int y) {
        int xx = x;
        int yy = y;

        if(xx < 0) xx = 0;
        if(yy < 0) yy = 0;
        if(xx >= chunks_x - 1) xx = chunks_x - 1;
        if(yy >= chunks_y - 1) yy = chunks_y - 1;

        if(xx >= 0) {
           return gridPos.at(abs(yy)).at(xx);         
        }
        return gridNeg.at(abs(yy)).at(xx);
    }

    Chunk& getChunkAt(const sf::Vector2i &pos) {
        if(pos.x >= 0) {
           return gridPos.at(abs(pos.y)).at(pos.x);         
        }
        return gridNeg.at(abs(pos.y)).at(pos.x);
    }

    void boundVector(sf::Vector2i &v) {
        if(v.y < 0) v.y = 0;
        if(v.x < -chunks_negx) v.x = 0;
        if(v.y > worldSize::world_sy - 1) v.y = worldSize::world_sy - 1;
        if(v.x > worldSize::world_sx - 1) v.x = worldSize::world_sx - 1;
    }

    const sf::Vector2i getBoundedVector(const sf::Vector2i &p) {
        sf::Vector2i v = p;

        if(v.y < 0) v.y = 0;
        if(v.x < -chunks_negx) v.x = 0;
        if(v.y > worldSize::world_sy - 1) v.y = worldSize::world_sy - 1;
        if(v.x > worldSize::world_sx - 1) v.x = worldSize::world_sx - 1;

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

        getChunkAt(pos.x/Chunk::sizeX, pos.y/Chunk::sizeY).getImage().setPixel(pos.x%Chunk::sizeX, pos.y%Chunk::sizeY, color);
    }

    const sf::Color getImagePixelAt(const int x, const int y) {

        return getChunkAt(x/Chunk::sizeX, y/Chunk::sizeY).getImage().getPixel(x%Chunk::sizeX, y%Chunk::sizeY);
    }

    void clearVoxelAt(const int x, const int y) {
        getVoxelAt(x,y).value = 0; 
        setImagePixelAt(x,y,sf::Color(0,0,0,0));
    }

    void boundDamageVoxelAt(const int xx, const int yy) {
        sf::Vector2i pos = sf::Vector2i(xx, yy);
        boundVector(pos);

        uint8_t &strenght = getVoxelAt(pos.x, pos.y).strenght;
        --strenght;
        if(getVoxelAt(pos.x, pos.y).strenght <= 0) { 
            clearVoxelAt(pos.x, pos.y);
        }

        if(getVoxelAt(pos.x, pos.y).value == VoxelValues::CARBON) materialpack.carbon += 1;
        else if(getVoxelAt(pos.x, pos.y).value == VoxelValues::LITHIUM) materialpack.lithium += 1;
        else if(getVoxelAt(pos.x, pos.y).value == VoxelValues::MAGNESIUM) materialpack.magnesium += 1;
        else if(getVoxelAt(pos.x, pos.y).value == VoxelValues::SODIUM) materialpack.sodium += 1;
        else if(getVoxelAt(pos.x, pos.y).value == VoxelValues::ALUMINIUM) materialpack.aluminium += 1;
        else if(getVoxelAt(pos.x, pos.y).value == VoxelValues::SILICON) materialpack.silicon += 1;
        else if(getVoxelAt(pos.x, pos.y).value == VoxelValues::COPPER) materialpack.copper += 1;
        else if(getVoxelAt(pos.x, pos.y).value == VoxelValues::TITANIUM) materialpack.titanium += 1;
        else if(getVoxelAt(pos.x, pos.y).value == VoxelValues::LEAD) materialpack.lead += 1;
    }
    
    void damageVoxelAt(const int x, const int y) {
        uint8_t &strenght = getVoxelAt(x,y).strenght;
        --strenght;
        if(getVoxelAt(x,y).strenght <= 0) { 
            clearVoxelAt(x,y);
        }
    }


    std::pair<int, sf::Vector2f> getPixelCollision(const sf::Vector2f& pos) {
        std::pair<int, sf::Vector2f> ret = {false, {0.f, 0.f}};
        sf::Vector2i pixelPosition = sf::Vector2i(pos);

        boundVector(pixelPosition);

        int result = 1;
        const sf::Color pixel = getImagePixelAt(pixelPosition.x, pixelPosition.y);

        result = pixel.a != 0;

        if(pixel == elm::getInfoFromType(VoxelValues::SNOW).color)
            result = 0;

        if(pixel == elm::getInfoFromType(VoxelValues::WATER).color)
            result = 5;

        ret.second = pos - sf::Vector2f(pixelPosition);

        ret.first = result;
        return ret;
    }


    void setImagePixelAt(const int x, const int y, const sf::Color& color) {
        getChunkAt(getChunkFromPos(x, y).x, getChunkFromPos(x, y).y).modified = true;

        getChunkAt(x/Chunk::sizeX, y/Chunk::sizeY).getImage().setPixel(x%Chunk::sizeX, y%Chunk::sizeY, color);
    }

    const bool isInContactWithVoxel(const sf::Vector2i &pos, const uint8_t voxelValue) {
        return (
            boundGetVoxelAt(pos.x + 1, pos.y).value == voxelValue
         || boundGetVoxelAt(pos.x - 1, pos.y).value == voxelValue
         || boundGetVoxelAt(pos.x, pos.y + 1).value == voxelValue
         || boundGetVoxelAt(pos.x, pos.y -1).value == voxelValue);
    }

    void SetHeat(const uint64_t x, const uint64_t y, int64_t temp)
    {
        Voxel &vox = getVoxelAt(x, y);

        if(vox.temp >= elm::getInfoFromType(vox.value).max_temp) {
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

        if(vox.temp >= elm::getInfoFromType(vox.value).max_temp) {
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

    void boundHeatVoxelAt(const int xx, const int yy, const int temp)
    {
        sf::Vector2i positon = sf::Vector2i(xx,yy);
        boundVector(positon);

        Voxel &vox = getVoxelAt(positon.x, positon.y);
        getChunkAt(getChunkFromPos(positon.x, positon.y).x, getChunkFromPos(positon.x, positon.y).y).modified = true;

        if(vox.temp >= elm::getInfoFromType(vox.value).max_temp) {
            uint8_t &strenght = vox.strenght;
            --strenght;
            if(vox.strenght <= 0) { 
                getVoxelAt(positon.x,positon.y).value = 0; 
                setImagePixelAt(positon.x, positon.y, sf::Color(0,0,0,0));
            }
        }

        vox.temp += temp;

        if(vox.temp <= 0) vox.temp = 0;

        sf::Color currPixel = getImagePixelAt(positon.x, positon.y);

        uint64_t valR = vox.temp * 1; 
        if(valR >= 255) valR = 255;
        currPixel.r = valR;
        if(valR > 300) {
            currPixel.g = 255;
            currPixel.b = 255;
        }

        setImagePixelAt(positon.x, positon.y,currPixel);
    }

    int64_t world_snegx;
    int64_t world_snegy;

    MaterialPack materialpack {};

private:
    std::vector<std::vector<Chunk>> gridPos;
    std::array<std::array<Chunk, chunks_negx + 1>, chunks_negy + 1> gridNeg;

};
