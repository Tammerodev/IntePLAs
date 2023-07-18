#pragma once
#include "Voxel.hpp"
#include <SFML/Graphics.hpp>

static int chunks_x = 64;
static int chunks_y = 64;

struct Chunk {
    std::vector <sf::Sprite> rects;
    std::array<std::array<Voxel, 64>, 64> arr;

    static const int sizeX = 64; 
    static const int sizeY = 64;

    bool loaded = true;

    std::array<std::array<Voxel, 64>, 64>& requestAccess() {
        if(loaded) return arr;
        else load();
    }

    sf::Image &requestImageAccess() {
        if(loaded) return image;
        else load();
    }

    void create(sf::Image &img, sf::Vector2i pos) {
        // Fill the array 
        image.create(sizeX,sizeY);
        image.copy(img, 0, 0, sf::IntRect(sf::Vector2i(pos.x * sizeX, pos.y * sizeY), sf::Vector2i(sizeX, sizeY)));
        tx.loadFromImage(image);
    }

    bool load() {
        loaded = true;
        return true;
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

struct ChunkArea {
    ChunkArea(const int32_t sX, const int32_t sY, const int32_t eX, const int32_t eY) : startX(sX), startY(sY), endX(eX), endY(eY) {

    }
    uint32_t startX = 0;
    uint32_t startY = 0;

    uint32_t endX = 0;
    uint32_t endY = 0;
};

struct ChunkBounds {
public:
    ChunkBounds(int32_t sX, int32_t sY, int32_t eX, int32_t eY) { 
        set(sX, sY, eX, eY);
    }

    void set(int32_t sX, int32_t sY, int32_t eX, int32_t eY) {
        if(sX < 0) sX = 0;
        if(sY < 0) sY = 0;
        if(eX < 0) eX = 0;
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