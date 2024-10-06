#pragma once
#include "Chunk.hpp"
#include "VoxelContainer.hpp"
#include "MaterialPack.hpp"
#include <math.h>

namespace worldSize {
    inline int64_t world_sx;
    inline int64_t world_sy;
}

class ChunkIndexer {
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
        worldSize::world_sx = Chunk::sizeX * chunks_x;
        worldSize::world_sy = Chunk::sizeY * chunks_y;
    }

    Chunk& getChunkAt(const int x, const int y) {
        return gridPos.at(y).at(x);         
    }

    Chunk& boundGetChunkAt(int x, int y) {
        if(x < 0) x = 0;
        if(y < 0) y = 0;
        if(x >= chunks_x - 1) x = chunks_x - 1;
        if(y >= chunks_y - 1) y = chunks_y - 1;

        return gridPos.at(y).at(x);         
    }

    Chunk& getChunkAt(sf::Vector2i pos) {
        if(pos.x > worldSize::world_sx - 1) pos.x = worldSize::world_sx - 1;
        if(pos.y > worldSize::world_sy - 1) pos.y = worldSize::world_sy - 1;
        if(pos.x < 0) pos.x = 0;
        if(pos.y < 0) pos.y = 0;


        return gridPos.at(pos.y).at(pos.x);         

    }

    void boundVector(sf::Vector2i &v) {
        if(v.y < 0) v.y = 0;

        if(v.x < 0) v.x = 0;

        if(v.y > worldSize::world_sy - 1) v.y = worldSize::world_sy - 1;

        if(v.x > worldSize::world_sx - 1) v.x = worldSize::world_sx - 1;
    }

    const sf::Vector2i getBoundedVector(const sf::Vector2i &p) {
        sf::Vector2i v = p;

        if(v.y < 0) v.y = 0;
        if(v.x < 0) v.x = 0;
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
        sf::Vector2i pos {x, y};

        boundVector(pos);

        return getChunkAt(pos.x/Chunk::sizeX, pos.y/Chunk::sizeY).arr[abs(pos.x%Chunk::sizeX)][abs(pos.y%Chunk::sizeY)];
    }


    void boundSetImagePixelAt(const int xx, const int yx, const sf::Color& color) {
        sf::Vector2i pos = sf::Vector2i(xx, yx);
        boundVector(pos);

        boundGetChunkAt(getChunkFromPos(pos.x, pos.y).x, getChunkFromPos(pos.x, pos.y).y).modified = true;

        boundGetChunkAt(pos.x/Chunk::sizeX, pos.y/Chunk::sizeY).getImage().setPixel(pos.x%Chunk::sizeX, pos.y%Chunk::sizeY, color);
    }

    const sf::Color getImagePixelAt(const int x, const int y) {

        return getChunkAt(x/Chunk::sizeX, y/Chunk::sizeY).getImage().getPixel(x%Chunk::sizeX, y%Chunk::sizeY);
    }

    void clearVoxelAt(const int x, const int y) {
        Voxel& vox = getVoxelAt(x,y);
        vox.value = 0; 
        vox.temp = 0; 
        setImagePixelAt(x, y, sf::Color(0,0,0,0));
    }

    void boundClearVoxelAt(const int x, const int y) {
        Voxel& vox = boundGetVoxelAt(x,y);
        vox.value = 0; 
        vox.temp = 0; 
        boundSetImagePixelAt(x, y, sf::Color(0,0,0,0));
    }

    void boundDamageVoxelAt(const int xx, const int yy) {
        sf::Vector2i pos = sf::Vector2i(xx, yy);
        boundVector(pos);

        uint8_t &strenght = getVoxelAt(pos.x, pos.y).strenght;
        --strenght;
        if(getVoxelAt(pos.x, pos.y).strenght <= 0) { 
            materialpack.addElementOfType(getVoxelAt(pos.x, pos.y).value, 1); 
            clearVoxelAt(pos.x, pos.y);
        }
    }

    bool doesLineContainMaterial(sf::Vector2i start, sf::Vector2i end) {

        boundVector(start);
        boundVector(end);


        sf::Vector2i delta = end - start;
        int length = static_cast<int>(std::sqrt(delta.x * delta.x + delta.y * delta.y));

        for (int i = 0; i <= length; i++) {
            float t = static_cast<float>(i) / length;
            sf::Vector2i pixelPosition(
                start.x + static_cast<unsigned>(delta.x * t),
                start.y + static_cast<unsigned>(delta.y * t)
            );

            if(getPixelCollision(sf::Vector2f(pixelPosition)).first == 1) {
                return true;
            }
        }
        return false;
    }

    sf::Vector2i pointLineContainMaterial(sf::Vector2i start, sf::Vector2i end) {
        boundVector(start);
        boundVector(end);

        sf::Vector2i delta = end - start;
        int length = static_cast<int>(std::sqrt(delta.x * delta.x + delta.y * delta.y));

        sf::Vector2i prev_pos = start;

        for (int i = 1; i <= length; i++) {

            if(i > worldSize::world_sx) {
                prndd("pointLineContainMaterial() encountered an unexpected loop size larger than world size!");
                return {0, 0};
            }

            const float t = static_cast<float>(i) / length;

            sf::Vector2i pixelPosition(
                start.x + static_cast<unsigned>(delta.x * t),
                start.y + static_cast<unsigned>(delta.y * t)
            );

            if(boundGetVoxelAt(pixelPosition.x, pixelPosition.y).value != 0) {
                return prev_pos;
            }

            prev_pos = pixelPosition;
        }
        return sf::Vector2i(0, 0);
    }
    
    void damageVoxelAt(const int x, const int y) {
        uint8_t &strenght = getVoxelAt(x,y).strenght;
        --strenght;
        
        if(getVoxelAt(x,y).strenght <= 0) { 
            materialpack.addElementOfType(getVoxelAt(x, y).value, 1); 
            clearVoxelAt(x, y);
        }
    }
    
    enum CollisionType {
        No, Yes, Fluid, RemovePart
    };

    std::pair<CollisionType, sf::Vector2f> getPixelCollision(const sf::Vector2f& pos) {
        std::pair<CollisionType, sf::Vector2f> ret = {CollisionType::No, {0.f, 0.f}};
        sf::Vector2i pixelPosition = sf::Vector2i(pos);

        boundVector(pixelPosition);

        CollisionType result = CollisionType::Yes;
        const sf::Color pixel = getImagePixelAt(pixelPosition.x, pixelPosition.y);
        const int voxel_value = getVoxelAt(pixelPosition.x, pixelPosition.y).value;

        if(pixel.a == 0) {
            result = CollisionType::No;
        }

        if(voxel_value == VoxelValues::SNOW)
            result = CollisionType::No;
        else if(voxel_value == VoxelValues::WATER || voxel_value == VoxelValues::NITROGLYCERIN || voxel_value == VoxelValues::BLOOD)
            result = CollisionType::Fluid;
        else if(voxel_value == VoxelValues::ACID) 
            result = CollisionType::RemovePart;

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
        if(vox.value == 0) return;

        if(vox.temp >= elm::getInfoFromType(vox.value).max_temp) {
            uint8_t &strenght = vox.strenght;
            --strenght;
            if(vox.strenght <= 0) { 
                clearVoxelAt(x, y);
            }
        }

        vox.temp = temp;

        if(temp <= 0) vox.temp = 0;

        sf::Color currPixel = getImagePixelAt(x,y);

        uint64_t valR = vox.temp * 1; 
        if(valR >= 255) valR = 255;
        currPixel.r = valR;
        if(valR > 300) {
            currPixel.g = 255;
            currPixel.b = 255;
        }

        setImagePixelAt(x, y, currPixel);
    }

    void heatVoxelAt(const int x, const int y, const int temp)
    {
        Voxel &vox = getVoxelAt(x,y);

        vox.temp += temp;
        if(vox.temp >= elm::getInfoFromType(vox.value).max_temp) {
            int val = vox.value;
            damageVoxelAt(x,y);
        }

        voxelsInNeedOfUpdate.emplace_back(x,y);

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
        sf::Vector2i position = sf::Vector2i(xx,yy);
        boundVector(position);

        Voxel &vox = getVoxelAt(position.x, position.y);
        if(vox.value == 0) return;

        getChunkAt(getChunkFromPos(position.x, position.y).x, getChunkFromPos(position.x, position.y).y).modified = true;

        if(vox.temp >= elm::getInfoFromType(vox.value).max_temp) {
            uint8_t &strenght = vox.strenght;
            --strenght;
            if(vox.strenght <= 0) { 
                clearVoxelAt(position.x, position.y);
            }
        }

        vox.temp += temp;

        if(vox.temp <= 0) vox.temp = 0;

        sf::Color currPixel = getImagePixelAt(position.x, position.y);

        uint64_t valR = vox.temp * 1; 
        if(valR >= 255) valR = 255;
        currPixel.r = valR;
        if(valR > 300) {
            currPixel.g = 255;
            currPixel.b = 255;
        }

        setImagePixelAt(position.x, position.y,currPixel);
    }

    void boundHeatVoxelAtAndAdd(const int xx, const int yy, const int temp)
    {
        sf::Vector2i position = sf::Vector2i(xx,yy);
        boundVector(position);

        Voxel &vox = getVoxelAt(position.x, position.y);
        if(vox.value == 0) return;

        getChunkAt(getChunkFromPos(position.x, position.y).x, getChunkFromPos(position.x, position.y).y).modified = true;

        if(vox.temp >= elm::getInfoFromType(vox.value).max_temp) {
            uint8_t &strenght = vox.strenght;
            --strenght;
            
            if(vox.strenght <= 0) { 
                clearVoxelAt(position.x, position.y);
            }
        }

        vox.temp += temp;

        if(vox.temp <= 0) vox.temp = 0;
        //else voxelsInNeedOfUpdate.emplace_back(position);

        sf::Color currPixel = getImagePixelAt(position.x, position.y);

        uint64_t valR = vox.temp * 1; 
        if(valR >= 255) valR = 255;
        currPixel.r = valR;
        if(valR > 300) {
            currPixel.g = 255;
            currPixel.b = 255;
        }

        setImagePixelAt(position.x, position.y,currPixel);
    }

    int64_t world_snegx;
    int64_t world_snegy;

    MaterialPack materialpack {};

    std::list<sf::Vector2i> voxelsInNeedOfUpdate;
    std::list<sf::Vector2i> reactiveVoxels;

private:
    std::vector<std::vector<Chunk>> gridPos;
};
