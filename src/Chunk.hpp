#pragma once
#include "Voxel.hpp"
#include <SFML/Graphics.hpp>

static int chunks_x = 64;
static int chunks_y = 64;

static int chunks_negx = 64;
static int chunks_negy = 64;


struct Chunk {
    std::vector <sf::Sprite> rects;

    static const int sizeX = 64; 
    static const int sizeY = 64;

    std::array<std::array<Voxel, 64>, 64> arr;
    bool loaded = true;
    
    std::array<std::array<Voxel, 64>, 64>& requestAccess() {
        if(loaded) return arr;
        else return load();
    }

    sf::Image &requestImageAccess() {
        if(loaded) return image;
        else load();
        return image;
    }

    void create() {
        image.create(sizeX, sizeY, sf::Color(0,0,0,0));
        tx.create(sizeX, sizeY);
    }

    std::array<std::array<Voxel, 64>, 64>& load() {
        loaded = true;
        return arr;
    }

    void update() {
        tx.update(image);
    }

    void unload() {
        // Remove the array from memory
        if(!arr.empty()) {
            rects.clear();
            loaded = false;
        }
    }

    sf::Image image;
    sf::Texture tx;
};

class ChunkIndexer {
public:
    ChunkIndexer() : gridPos(chunks_y + 1, std::vector<Chunk>(chunks_x + 1)), 
                    gridNeg(chunks_negy + 1, std::vector<Chunk>(chunks_negx + 1))  {
    }

    Chunk &getChunkAt(int64_t x, int64_t y) {
        
        if(x < 0) {
           return gridNeg[abs(x)][y];         
        }
        return gridPos[abs(x)][y];
    }
private:
    std::vector<std::vector<Chunk>> gridPos;
    std::vector<std::vector<Chunk>> gridNeg;
};

struct ChunkArea {
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
        if(sY < -chunks_negy) sY = -chunks_negy;

        if(eX < -chunks_negx) eX = -chunks_negx;
        if(eY < -chunks_negy) eY = -chunks_negy;

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