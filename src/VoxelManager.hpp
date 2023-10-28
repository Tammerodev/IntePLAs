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
#include "Element.hpp"
#include "Fluid.hpp"
#include "Water.hpp"
#include "Nitroglycerin.hpp"
#include "Chlorine.hpp"
#include "Shader.hpp"
#include "Burning.hpp"
#include "Settings.hpp"

#include <list>

class VoxelManager {
public:

    VoxelManager() : chIndexer() {

    }

    bool getPixelCollision(sf::Vector2i pos);

    int load(std::string, bool);

    void initVoxelMap() {
        for (int y = 0;y < chIndexer.world_sy;y++) {
            for (int x = 0;x < chIndexer.world_sx;x++) {
                const sf::Color px = chIndexer.getImagePixelAt(x,y);
                chIndexer.getVoxelAt(x,y) = getValueFromCol(px, sf::Vector2i(x,y));
            }
        }
    }

    void clearVoxelAt(const uint64_t x, const uint64_t y) {
        chIndexer.getVoxelAt(x,y).value = 0; 
        chIndexer.setImagePixelAt(x,y,sf::Color(0,0,0,0));
    }

    void damageVoxelAt(const uint64_t x, const uint64_t y) {
        uint8_t &strenght = chIndexer.getVoxelAt(x,y).strenght;
        --strenght;
        if(chIndexer.getVoxelAt(x,y).strenght <= 0) { 
            // I know this is terrible
            if(chIndexer.getVoxelAt(x, y).value == elm::ValCarbon) materialpack.carbon += 1;
            else if(chIndexer.getVoxelAt(x, y).value == elm::ValLithium) materialpack.lithium += 1;
            else if(chIndexer.getVoxelAt(x, y).value == elm::ValMagnesium) materialpack.magnesium += 1;
            else if(chIndexer.getVoxelAt(x, y).value == elm::ValSodium) materialpack.sodium += 1;
            else if(chIndexer.getVoxelAt(x, y).value == elm::ValAluminium) materialpack.aluminium += 1;
            else if(chIndexer.getVoxelAt(x, y).value == elm::ValSilicon) materialpack.silicon += 1;
            else if(chIndexer.getVoxelAt(x, y).value == elm::ValCopper) materialpack.copper += 1;
            else if(chIndexer.getVoxelAt(x, y).value == elm::ValTitanium) materialpack.titanium += 1;
            else if(chIndexer.getVoxelAt(x, y).value == elm::ValLead) materialpack.lead += 1;

            clearVoxelAt(x,y);
        }
    }

    void heatVoxelAt(const uint64_t x, const uint64_t y, int64_t temp);
    void render(sf::RenderTarget&, const sf::Vector2f &center);
    void update();
    void hole(sf::Vector2i pos, const uint32_t intensity, bool force, const int64_t heat);

    bool generate();
    bool generateVegetation();

    void save() {
        // TODO : make it save from the chunk images
        //img.saveToFile("res/saves/" + std::to_string(time(0)) + ".png");
    }

    const bool compareVoxelColors(const sf::Color &color1, const sf::Color &color2) {
        return (color1.r == color2.r &&
                color1.g == color2.g &&
                color1.b == color2.b);
    }

    const bool isVoxelPosInView(const sf::Vector2i &pos) {
        return (Controls::gameCameraCenterPos.x - pos.x < 500.0) &&
                (Controls::gameCameraCenterPos.y - pos.y < 500.0) &&
                (Controls::gameCameraCenterPos.x + pos.x < 500.0) &&
                (Controls::gameCameraCenterPos.y + pos.y < 500.0);
    }

    const bool isInContactWithVoxel(const sf::Vector2i &pos, const uint8_t voxelValue) {
        return (chIndexer.getVoxelAt(pos.x + 1, pos.y).value == voxelValue || chIndexer.getVoxelAt(pos.x - 1, pos.y).value == voxelValue ||
            chIndexer.getVoxelAt(pos.x, pos.y + 1).value == voxelValue || chIndexer.getVoxelAt(pos.x, pos.y -1).value == voxelValue);
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
            vox.value = elm::ValWater;
            vox.strenght = 2;

            if(addVoxelsToArr) elements.push_back(std::make_shared<Water>(p.x, p.y));
        } else if(px == elm::Nitroglycerin) {
            vox.value = elm::ValNitroglycerin;
            vox.strenght = 2;

            if(addVoxelsToArr) elements.push_back(std::make_shared<Nitroglycerin>(p.x, p.y));
        } else if(px == elm::Chlorine) {
            vox.value = elm::ValChlorine;
            vox.strenght = 2;

            if(addVoxelsToArr) elements.push_back(std::make_shared<Chlorine>(p.x, p.y));
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

    Shader shader;

    sf::Text text;
    sf::Font font;

    ProcGenerate procGen;

    MaterialPack materialpack;

    ChunkIndexer chIndexer;

    std::list<sf::Vector2i> voxelsInNeedOfUpdate;
    std::vector<sf::Vector2i> mergeChunks;

    std::list<sf::Vector2i> reactiveVoxels;

    std::list<std::shared_ptr<Element>> elements;
    // These have to be specified here for efficiency

    ChunkBounds draw_bounds { 0, 0, 0, 0 };
    ChunkArea draw_area;
    sf::Sprite spriteRend;
};