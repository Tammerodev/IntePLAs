#pragma once
#include "Voxel.hpp"
#include <SFML/Graphics.hpp>

inline int chunks_x = 128;
inline int chunks_y = 20;

static const int chunks_negx = 0;
static const int chunks_negy = 0;

struct Chunk {

    static const int sizeX = 64; 
    static const int sizeY = 64;

    std::array<std::array<Voxel, 64>, 64> arr;

    sf::Image &requestImageAccess() {
        return image;
    }

    void create() {
        image.create(Chunk::sizeX, Chunk::sizeY, sf::Color(0,0,0,0));
        tx.create(Chunk::sizeX, Chunk::sizeY);
    }

    void load(const char *file_path) {
        image.loadFromFile(file_path);
        tx.loadFromFile(file_path);
    }

    void update() {
        tx.update(image);
        modified = false;
    }

    sf::Image image;
    sf::Texture tx;

    bool modified = true;
};

class ChunkIndexer {
public:
    ChunkIndexer() {
        updateWorldSize();
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

    void updateWorldSize() {
        world_sx = Chunk::sizeX * chunks_x;
        world_sy = Chunk::sizeY * chunks_y;

        world_snegx = Chunk::sizeX * chunks_negx;
        world_snegy = Chunk::sizeY * chunks_negy;
    }

    Chunk &getChunkAt(int64_t x, int64_t y) {
        if(x >= 0) {
           return gridPos.at(abs(y)).at(x);         
        }
        return gridNeg.at(abs(y)).at(x);
    }

    Chunk &getChunkAt(const sf::Vector2i &pos) {
        if(pos.x >= 0) {
           return gridPos.at(abs(pos.y)).at(pos.x);         
        }
        return gridNeg.at(abs(pos.y)).at(pos.x);
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

    const sf::Vector2i getChunkFromPos(int x, int y) {
        return sf::Vector2i(x/Chunk::sizeX, y/Chunk::sizeY);
    }

    Voxel &getVoxelAt (const int64_t x, const int64_t y) {
        return getChunkAt(x/Chunk::sizeX, y/Chunk::sizeY).arr[abs(x%Chunk::sizeX)][abs(y%Chunk::sizeY)];
    }
    
    Voxel &boundGetVoxelAt (const int64_t x, const int64_t y) {
        sf::Vector2i pos = sf::Vector2i(x, y);

        boundVector(pos);

        return getChunkAt(pos.x/Chunk::sizeX, pos.y/Chunk::sizeY).arr[abs(pos.x%Chunk::sizeX)][abs(pos.y%Chunk::sizeY)];
    }


    void boundSetImagePixelAt(const uint64_t x, const uint64_t y, const sf::Color& color) {
        sf::Vector2i pos = sf::Vector2i(x, y);
        getChunkAt(getChunkFromPos(x, y)).modified = true;

        boundVector(pos);

        getChunkAt(pos.x/Chunk::sizeX, pos.y/Chunk::sizeY).image.setPixel(pos.x%Chunk::sizeX, pos.y%Chunk::sizeY, color);
    }

    const sf::Color getImagePixelAt(const uint64_t x, const uint64_t y) {

        return getChunkAt(x/Chunk::sizeX, y/Chunk::sizeY).image.getPixel(x%Chunk::sizeX, y%Chunk::sizeY);
    }

    void clearVoxelAt(const uint64_t x, const uint64_t y) {
        getVoxelAt(x,y).value = 0; 
        setImagePixelAt(x,y,sf::Color(0,0,0,0));
    }
    
    void damageVoxelAt(const uint64_t x, const uint64_t y) {
        uint8_t &strenght = getVoxelAt(x,y).strenght;
        --strenght;
        if(getVoxelAt(x,y).strenght <= 0) { 
            clearVoxelAt(x,y);
        }
    }

    void setImagePixelAt(const uint64_t x, const uint64_t y, const sf::Color& color) {
        getChunkAt(getChunkFromPos(x, y)).modified = true;
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

    void Heat(const uint64_t x, const uint64_t y, int64_t temp)
    {
        Voxel &vox = getVoxelAt(x, y);
        getChunkAt(getChunkFromPos(x, y)).modified = true;

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

struct ChunkArea {
    ChunkArea() = default;
    ChunkArea(const int64_t sX, const int64_t sY, const int64_t eX, const int64_t eY) : startX(sX), startY(sY), endX(eX), endY(eY) {

    }
    int64_t startX = 0;
    int64_t startY = 0;

    int64_t endX = 0;
    int64_t endY = 0;
};

struct ChunkBounds {
public:
    ChunkBounds(int64_t sX, int64_t sY, int64_t eX, int64_t eY) { 
        set(sX, sY, eX, eY);
    }

    void set(int64_t sX, int64_t sY, int64_t eX, int64_t eY) {

        if(sX < -chunks_negx) sX = -chunks_negx;
        if(sY < 0) sY = 0;

        if(eX < -chunks_negx) eX = -chunks_negx;
        if(eY < 0) eY = 0;

        if(sX >= chunks_x) sX = chunks_x;
        if(sY >= chunks_y) sY = chunks_y;
        if(eX >= chunks_x) eX = chunks_x;
        if(eY >= chunks_y) eY = chunks_y;

        area = ChunkArea(sX, sY, eX, eY);
    }

    const ChunkArea &getArea() const {
        return area;
    }
private:
    ChunkArea area {0,0,0,0};
};