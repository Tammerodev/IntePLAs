#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <math.h>
#include <thread>
#include <optional>
#include <future>
#include <filesystem>

#include "math.hpp"
#include "Voxel.hpp"

#include "ExplosionInfo.hpp"
#include "common.hpp"
#include "Elements.hpp"
#include "Chunk.hpp"
#include "ChunkIndexerVoxelContainer.hpp"

#include "MaterialPack.hpp"
#include "VoxelGroup.hpp"
#include "ProceduralGeneration.hpp"
#include "Controls.hpp"
#include "Element.hpp"
#include "Acid.hpp"
#include "Fluid.hpp"
#include "Water.hpp"
#include "Nitroglycerin.hpp"
#include "Chlorine.hpp"
#include "Shader.hpp"
#include "Burning.hpp"
#include "Settings.hpp"
#include "Sand.hpp"
#include "Raycast.hpp"

#include "Uranium-235.hpp"
#include "Radium-226.hpp"

#include "Player.hpp"

#include "ParticleSimulation.hpp"
#include "VoxelSpy.hpp"
#include "ClientManager.hpp"
#include "Session.hpp"

#include <list>
#include <future>


class VoxelManager {
public:

    VoxelManager() : chIndexer() {

    }

    std::pair<bool, sf::Vector2f> getPixelCollision(const sf::Vector2f& pos);

    int load(std::string);

    void initVoxelMap() {
        for (int y = 0;y < chIndexer.world_sy;y++) {
            for (int x = 0;x < chIndexer.world_sx;x++) {
                const sf::Color px = chIndexer.getImagePixelAt(x,y);
                chIndexer.getVoxelAt(x,y) = getHandleVoxel(px, sf::Vector2i(x,y), true);
            }
        }
    }

    const std::string getPath(int x, int y) {
        const std::string x_string  = std::to_string(x);
        const std::string y_string  = std::to_string(y);

        const std::string filename = x_string + "_" + y_string;
        const std::string extension = ".png";

        return filename + extension;
    }

    void heatVoxelAt(const uint64_t x, const uint64_t y, int64_t temp);
    void render(sf::RenderTarget&, const sf::Vector2f &center);
    void update(Player&);
    void hole(sf::Vector2i pos, const uint32_t intensity, bool force, const int64_t heat);
    void holeRayCast(sf::Vector2i pos, const uint32_t intensity, bool force, const int64_t heat);

    void explosionEffect(const sf::Vector2f &p, int intensity) {
        if(explosion_points.size() < PREALLOCATE_EFFECTS_COUNT) {
            ExplosionInfo info;
            info.position = sf::Vector2f(p);
            info.strength = intensity;
            explosion_points.push_back(info);
        }
    }

    void launchDebrisParticle(const sf::Vector2i& p, const sf::Color &col, sf::Vector2f vel = sf::Vector2f(0.f, 0.f), float size = 1.f) {
        if(!GraphicsSettings::particleEffects) return;
        
        const sf::Vector2f position = sf::Vector2f(p); 
        sf::Vector2f velocity = sf::Vector2f(math::randFloat() - 0.5f, math::randFloat() - 0.5f) * 10.0f; 
        velocity = vel;

        std::shared_ptr<Debris> particle = std::make_shared<Debris>(position, velocity, col, size);

        particleSimulation.addParticle(particle);
    }

    void launchParticle(std::shared_ptr<Particle> particle) {
        if(!GraphicsSettings::particleEffects) return;
        
        particleSimulation.addParticle(particle);
    }

    bool doesLineContainMaterial(const sf::Vector2i start, const sf::Vector2i end) {
        sf::Vector2i delta = end - start;
        int length = static_cast<int>(std::sqrt(delta.x * delta.x + delta.y * delta.y));

        for (int i = 0; i <= length; i++) {
            float t = static_cast<float>(i) / length;
            sf::Vector2i pixelPosition(
                start.x + static_cast<unsigned>(delta.x * t),
                start.y + static_cast<unsigned>(delta.y * t)
            );

            if(chIndexer.getVoxelAt(pixelPosition.x, pixelPosition.y).value != 0) {
                return true;
            }
        }
        return false;
    }

    bool generate();
    bool generateVegetation();

    void save() {
        const std::string created_folder = std::to_string(time(0));

        if (!std::filesystem::create_directory(StorageSettings::save_path + created_folder)) {
            prnerr("Could not create folder for save! Path is ", StorageSettings::save_path + created_folder);
            return;
        }

        std::vector<std::future<void>> futures;

        for (int y = 0; y < chunks_y; y++) {
            for (int x = 0; x < chunks_x; x++) {
                const std::string filename = std::to_string(x) + "_" + std::to_string(y);
                const std::string fullpath = StorageSettings::save_path + created_folder + "/" + filename;

                // Use std::async with a lambda function
                futures.emplace_back(std::async(std::launch::deferred, [fullpath, filename, this]() {
                    // Inside the lambda, save the chunk
                    this->getChunkIndexer().getChunkAt(std::stoi(filename), std::stoi(filename)).image.saveToFile(fullpath + ".png");
                }));
            }
        }

        prndd("Saving complete!!!11!!");

        // Wait for all threads to finish
        for (auto& future : futures) {
            future.wait();
        }
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

    const sf::Vector2i getPositionOnContacy(const sf::Vector2i &pos, const uint8_t voxelValue) {

        /*
            @brief This returns (0, 0) if no voxel found
        */
        const sf::Vector2i pos1(pos.x + 1, pos.y);
        const sf::Vector2i pos2(pos.x - 1, pos.y);
        const sf::Vector2i pos3(pos.x, pos.y + 1);
        const sf::Vector2i pos4(pos.x, pos.y - 1);

        if(chIndexer.getVoxelAt(pos1.x, pos1.y).value = voxelValue) 
            return pos1;

        if(chIndexer.getVoxelAt(pos2.x, pos2.y).value = voxelValue) 
            return pos2;

        if(chIndexer.getVoxelAt(pos3.x, pos3.y).value = voxelValue) 
            return pos3;

        if(chIndexer.getVoxelAt(pos4.x, pos4.y).value = voxelValue) 
            return pos4;

        return sf::Vector2i(0, 0);
    }

    const Voxel getHandleVoxel(const sf::Color &px, sf::Vector2i p, bool addVoxelsToArr = false) {
        Voxel vox = Voxel();
        vox.value = px.a != 0;
        if(vox.value == 0) return vox;

        if(px == elm::Carbon) {
            vox.value = 2;
            vox.strenght = 8;
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

            if(addVoxelsToArr) addElement(p.x, p.y, std::make_shared<Water>(p.x, p.y));
        } else if(px == elm::Acid) {
            vox.value = elm::ValAcid;
            vox.strenght = 2;

            if(addVoxelsToArr) addElement(p.x, p.y, std::make_shared<Acid>(p.x, p.y));
        }else if(px == elm::Nitroglycerin) {
            vox.value = elm::ValNitroglycerin;
            vox.strenght = 2;

            if(addVoxelsToArr) addElement(p.x, p.y, std::make_shared<Nitroglycerin>(p.x, p.y));
        } else if(px == elm::Chlorine) {
            vox.value = elm::ValChlorine;
            vox.strenght = 2;

            if(addVoxelsToArr) addElement(p.x, p.y, std::make_shared<Chlorine>(p.x, p.y));
        } else if(px == elm::Uranium235) {
            vox.value = elm::ValUranium235;
            vox.strenght = 2;

            if(addVoxelsToArr) radioactive_elements.push_back(std::make_shared<Uranium235>(p.x, p.y));
        }

        else if(px == elm::Radium226) {
            vox.value = elm::ValRadium226;
            vox.strenght = 2;

            if(addVoxelsToArr) radioactive_elements.push_back(std::make_shared<Radium226>(p.x, p.y));
        }

        else if(px == elm::Sand) {
            vox.value = elm::ValSand;
            vox.strenght = 2;

            

            if(addVoxelsToArr) addElement(p.x, p.y, std::make_shared<Sand>(p.x, p.y));
        }
        return vox;
    }

    void addElement(int x, int y, const std::shared_ptr<Element> element) {
        chIndexer.boundGetChunkAt(chIndexer.getChunkFromPos(x,y).x, chIndexer.getChunkFromPos(x,y).y).needs_update = true;

        chIndexer.boundGetChunkAt(chIndexer.getChunkFromPos(x, y).x, chIndexer.getChunkFromPos(x, y).y).elements.push_back(element);
    }


    MaterialPack &getReceivedMaterials() {
        return chIndexer.materialpack;
    }

    void valueFromColor() {

    }

    ChunkIndexer &getChunkIndexer() {
        return chIndexer;
    }

    void updateAll() {

        ChunkBounds bounds = ChunkBounds(0, 0, chunks_x, chunks_y);
        ChunkArea area = bounds.getArea();

        for(int64_t y = area.startY; y < area.endY; y++) {
            for(int64_t x = area.startX; x < area.endX; x++) {
                chIndexer.getChunkAt(x, y).update();

            }
        }
    }

    void build_image(const sf::Vector2i&, const sf::Image&, std::list<VoxelGroup>*, const sf::Vector2f velocity = sf::Vector2f(0.f, 0.f), bool hasCollisions = true);

    std::vector <ExplosionInfo> explosion_points;
    std::vector<sf::Vector2i> updateChunks;

    const sf::FloatRect &getUpdateArea() {
        return update_area;
    }

private:

    sf::FloatRect update_area;

    ClientManager clientManager;
    ProcGenerate procGen;
    VoxelSpy voxelSpy;
    ChunkIndexer chIndexer;

    std::vector<sf::Vector2i> mergeChunks;

    std::list<sf::Vector2i> voxelsInNeedOfUpdate;
    std::list<sf::Vector2i> reactiveVoxels;

    std::list<std::shared_ptr<RadioactiveElement>> radioactive_elements;

    ParticleSimulation particleSimulation;
};