#pragma once
#include "Voxel.hpp"
#include <SFML/Graphics.hpp>

static const int chunks_x = 1;
static const int chunks_y = 128;

static const int chunks_negx = 0;
static const int chunks_negy = 0;


struct Chunk {
    std::vector <sf::Sprite> rects;

    static const int sizeX = 64; 
    static const int sizeY = 64;

    std::array<std::array<Voxel, 64>, 64> arr;

    sf::Image &requestImageAccess() {
        return image;
    }

    void create() {
        image.create(sizeX, sizeY, sf::Color(0,0,0,0));
        tx.create(sizeX, sizeY);
    }

    void update() {
        tx.update(image);
    }

    sf::Image image;
    sf::Texture tx;
};

class ChunkIndexer {
public:
    ChunkIndexer() {

    }

    Chunk &getChunkAt(int64_t x, int64_t y) {

        if(x >= 0) {
           return gridPos.at(abs(y)).at(x);         
        }
        return gridNeg.at(abs(y)).at(x);
    }
private:
    std::array<std::array<Chunk, chunks_x + 1>, chunks_y + 1> gridPos;
    std::array<std::array<Chunk, chunks_negx + 1>, chunks_negy + 1> gridNeg;
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