#pragma once
#include "Voxel.hpp"
#include <SFML/Graphics.hpp>
#include <list>
#include <memory>
#include "SaveUtils.hpp"
#include "common.hpp"
#include <malloc.h>
#include "debug_globals.hpp"
class Element;
class RadioactiveElement;

inline int chunks_x = 128;
inline int chunks_y = 128;

static const int chunks_negx = 0;
static const int chunks_negy = 0;

struct Chunk {

    static const int sizeX = 16; 
    static const int sizeY = 16;

    std::array<std::array<Voxel, 16>, 16> arr;

    void create() {
        image = new sf::Image();
        image->create(Chunk::sizeX, Chunk::sizeY, sf::Color(0,0,0,0));
        tx.create(Chunk::sizeX, Chunk::sizeY);

        debug_globals::loaded_chunks++;
    }

    bool load(const char *file_path) {
        image = new sf::Image();

        bool success = true;
        if(!image->loadFromFile(file_path)) success = false;
        if(!tx.loadFromFile(file_path)) success = false;

        debug_globals::loaded_chunks++;

        return success;
    }

    void unLoad() {
        // Rare case where trying to unload unloaded chunk...
        if(image == nullptr) {
            return;
        }

        // Delete image
        image->saveToFile(utils::path + utils::getPath(position.x, position.y));
        delete image;
        image = nullptr;

        debug_globals::loaded_chunks--;
    }

    sf::Image &getImage() {
        if(image == nullptr) {
            load((utils::path + utils::getPath(position.x, position.y)).c_str());
        }
        return *image;
    }

    void update() {
        if(image == nullptr) {
            load((utils::path + utils::getPath(position.x, position.y)).c_str());
        }
        
        tx.update(*image);
        modified = false;
    }

    void setPosition(const sf::Vector2i &pos) {
        position = pos;
    }

    sf::Texture tx;

    bool modified = true;

private:

    sf::Image* image = nullptr;
    sf::Vector2i position {0, 0};

public:
    bool needs_update = false;

    std::list<std::shared_ptr<Element>> elements;
    std::list<std::shared_ptr<RadioactiveElement>> radioactive_elements;
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

    void set_forced(int64_t sX, int64_t sY, int64_t eX, int64_t eY) {
        area = ChunkArea(sX, sY, eX, eY);
    }

    const ChunkArea &getArea() const {
        return area;
    }
private:
    ChunkArea area {0,0,0,0};
};