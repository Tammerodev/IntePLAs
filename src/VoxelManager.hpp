#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <math.h>
#include <thread>
#include <optional>
#include <future>

#include "math.hpp"
#include "Voxel.hpp"

#include "ExplosionInfo.hpp"
#include "common.hpp"
#include "Elements.hpp"
#include "Chunk.hpp"
#include "MaterialPack.hpp"
#include "VoxelGroup.hpp"
#include "ProceduralGeneration.hpp"
#include "Controls.hpp"
#include <list>

class VoxelManager {
public:

    VoxelManager() : chIndexer() {

    }

    bool getPixelCollision(sf::Vector2i pos);

    int load(std::string, bool);

    void boundVector(sf::Vector2i &v) {
        if(v.y < 0) v.y = 0;
        if(v.x < -chunks_negx) v.x = 0;
        if(v.y > world_sy - 1) v.y = world_sy - 1;
        if(v.x > world_sx - 1) v.x = world_sx - 1;
    }

    void initVoxelMap() {
        for (int y = 0;y < world_sy;y++) {
            for (int x = 0;x < world_sx;x++) {
                const sf::Color px = getImagePixelAt(x,y);
                getVoxelAt(x,y) = getValueFromCol(px, sf::Vector2i(x,y));
            }
        }
    }

    void clearVoxelAt(const uint64_t x, const uint64_t y) {
        getVoxelAt(x,y).value = 0; 
        setImagePixelAt(x,y,sf::Color(0,0,0,0));
    }

    void damageVoxelAt(const uint64_t x, const uint64_t y) {
        uint8_t &strenght = getVoxelAt(x,y).strenght;
        --strenght;
        if(getVoxelAt(x,y).strenght <= 0) { 
            // I know this is terrible
            if(getVoxelAt(x, y).value == elm::ValCarbon) materialpack.carbon += 1;
            else if(getVoxelAt(x, y).value == elm::ValLithium) materialpack.lithium += 1;
            else if(getVoxelAt(x, y).value == elm::ValMagnesium) materialpack.magnesium += 1;
            else if(getVoxelAt(x, y).value == elm::ValSodium) materialpack.sodium += 1;
            else if(getVoxelAt(x, y).value == elm::ValAluminium) materialpack.aluminium += 1;
            else if(getVoxelAt(x, y).value == elm::ValSilicon) materialpack.silicon += 1;
            else if(getVoxelAt(x, y).value == elm::ValCopper) materialpack.copper += 1;
            else if(getVoxelAt(x, y).value == elm::ValTitanium) materialpack.titanium += 1;
            else if(getVoxelAt(x, y).value == elm::ValLead) materialpack.lead += 1;

            clearVoxelAt(x,y);
        }
    }

    void heatVoxelAt(const uint64_t x, const uint64_t y, int64_t temp);
    void render(sf::RenderTarget&, const sf::Vector2f &center);
    void resetUsedFlag();
    void update();
    void hole(const sf::Vector2i &pos, const uint32_t &intensity, bool force, const int64_t heat);

    bool generate();
    bool generateVegetation();

    void save() {
        // TODO : make it save from the chunk images
        //img.saveToFile("res/saves/" + std::to_string(time(0)) + ".png");
    }

    Voxel &getVoxelAt (const int64_t x, const int64_t y) {
        return chIndexer.getChunkAt(x/Chunk::sizeX, y/Chunk::sizeY).arr[abs(x%Chunk::sizeX)][abs(y%Chunk::sizeY)];
    }

    Voxel &boundGetVoxelAt (const int64_t x, const int64_t y) {
        sf::Vector2i pos = sf::Vector2i(x, y);

        boundVector(pos);


        return chIndexer.getChunkAt(pos.x/Chunk::sizeX, pos.y/Chunk::sizeY).arr[abs(pos.x%Chunk::sizeX)][abs(pos.y%Chunk::sizeY)];
    }

    const bool compareVoxelColors(const sf::Color &color1, const sf::Color &color2) {
        return (color1.r == color2.r &&
                color1.g == color2.g &&
                color1.b == color2.b);
    }

    const sf::Color getImagePixelAt(const uint64_t x, const uint64_t y) {
        return chIndexer.getChunkAt(x/Chunk::sizeX, y/Chunk::sizeY).image.getPixel(x%Chunk::sizeX, y%Chunk::sizeY);
    }

    void setImagePixelAt(const uint64_t x, const uint64_t y, const sf::Color& color) {
        chIndexer.getChunkAt(x/Chunk::sizeX, y/Chunk::sizeY).image.setPixel(x%Chunk::sizeX, y%Chunk::sizeY, color);
    }

    void boundSetImagePixelAt(const uint64_t x, const uint64_t y, const sf::Color& color) {
        sf::Vector2i pos = sf::Vector2i(x, y);

        boundVector(pos);

        chIndexer.getChunkAt(pos.x/Chunk::sizeX, pos.y/Chunk::sizeY).image.setPixel(pos.x%Chunk::sizeX, pos.y%Chunk::sizeY, color);
    }

    const bool isVoxelPosInView(const sf::Vector2i &pos) {
        return (Controls::gameCameraCenterPos.x - pos.x < 500.0) &&
                (Controls::gameCameraCenterPos.y - pos.y < 500.0) &&
                (Controls::gameCameraCenterPos.x + pos.x < 500.0) &&
                (Controls::gameCameraCenterPos.y + pos.y < 500.0);
    }

    const bool isInContactWithVoxel(const sf::Vector2i &pos, const uint8_t voxelValue) {
        return (getVoxelAt(pos.x + 1, pos.y).value == voxelValue || getVoxelAt(pos.x - 1, pos.y).value == voxelValue ||
            getVoxelAt(pos.x, pos.y + 1).value == voxelValue || getVoxelAt(pos.x, pos.y -1).value == voxelValue);
    }

    const Voxel getHandleVoxel(const sf::Color &px, sf::Vector2i p, bool addVoxelsToArr = false) {
        Voxel vox = Voxel();
        vox.value = px.a != 0;
        if(vox.value == 0) return vox;

        if(px == elm::Carbon) {
            vox.value = 2;
            vox.strenght = 254; // TODO change to 8
        } else if(px == elm::Lithium) {
            vox.value = 3;
            vox.strenght = 2;
            
            if(addVoxelsToArr) reactiveVoxels.push_back(p);
        } else if(px == elm::Magnesium) {
            vox.value = 4;
            vox.strenght = 10;
        } else if(px == elm::Sodium) {
            vox.value = 5;
            vox.strenght = 1;

            if(addVoxelsToArr) reactiveVoxels.push_back(p);
        } else if(px == elm::Aluminium) {
            vox.value = 6;
            vox.strenght = 5;
        } else if(px == elm::Silicon) {
            vox.value = 7;
            vox.strenght = 6;
        } else if(px == elm::Copper) {
            vox.value = 8;
            vox.strenght = 10;
        } else if(px == elm::Titanium) {
            vox.value = 9;
            vox.strenght = 100;
        } else if(px == elm::Lead) {
            vox.value = 10;
            vox.strenght = 3;
        } else if(px == elm::Water) {
            vox.value = 11;
            vox.strenght = 255;

            if(addVoxelsToArr) fluidVoxels.push_back(p);
        }

        return vox;
    }

    MaterialPack &getReceivedMaterials() {
        return materialpack;
    }

    void valueFromColor() {

    }

    void build_image(const sf::Vector2i&, const sf::Image&, std::list<VoxelGroup>*, float angle = 0.f, float mag = 0.f);

    std::vector <ExplosionInfo> explosion_points;
    std::vector<sf::Vector2i> updateChunks;

private:

    float burn_timer = 0.0f;

    sf::Text text;
    sf::Font font;

    ProcGenerate procGen;

    MaterialPack materialpack;

    ChunkIndexer chIndexer;

    std::list<sf::Vector2i> voxelsInNeedOfUpdate;
    std::vector<sf::Vector2i> mergeChunks;

    std::list<sf::Vector2i> burningVoxels;
    std::list<sf::Vector2i> fluidVoxels;
    std::list<sf::Vector2i> reactiveVoxels;

    int64_t world_sx;
    int64_t world_sy;

    int64_t world_snegx;
    int64_t world_snegy;

    // These have to be specified here for efficiency

    ChunkBounds draw_bounds { 0, 0, 0, 0 };
    ChunkArea draw_area;
    sf::Sprite spriteRend;
};