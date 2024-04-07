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

#include "Elements/Element.hpp"
#include "Elements/Acid.hpp"
#include "Elements/Water.hpp"
#include "Elements/Blood.hpp"
#include "Elements/Nitroglycerin.hpp"
#include "Elements/Chlorine.hpp"
#include "Elements/Sand.hpp"
#include "Elements/Wood.hpp"
#include "Elements/ManganeseHeptoxide.hpp"
#include "Elements/Butane.hpp"
#include "Elements/Snow.hpp"
#include "Elements/Flammable.hpp"

#include "Raycast.hpp"
#include "Shader.hpp"
#include "Settings.hpp"

#include "Uranium-235.hpp"
#include "Radium-226.hpp"

#include "Player.hpp"

#include "ParticleSimulation.hpp"
#include "Session.hpp"

#include <list>
#include <future>
#include "SimulationManager.hpp"

#include "SaveUtils.hpp"
#include "FireEffectManager.hpp"

#include "GameEventEnum.hpp"
#include "EventGlobals.hpp"

#include "Oscillator.hpp"
#include "Transistor.hpp"
#include "Switch.hpp"

class VoxelManager {
public:

    VoxelManager() : chIndexer() {

    }

    int load(std::string);

    void initVoxelMap() {
        for (int y = 0;y < worldSize::world_sy;y++) {
            for (int x = 0;x < worldSize::world_sx;x++) {
                const sf::Color px = chIndexer.getImagePixelAt(x,y);
                chIndexer.getVoxelAt(x,y) = getHandleVoxel(px, sf::Vector2i(x,y), true);
            }
        }
    }

    void heatVoxelAt(const uint64_t x, const uint64_t y, int64_t temp);
    void render(sf::RenderTarget&, const sf::Vector2f &center);
    void update(Player&, GameEventEnum::Event&);
    void hole(sf::Vector2i pos, const uint32_t intensity, bool force, const int64_t heat, const unsigned char collect_percent = 10);
    void holeRayCast(const sf::Vector2i& pos, const uint32_t intensity, bool force, const int64_t heat, const unsigned char collect_percent = 10, bool turnToAsh = false);
    void singleRayCast(const sf::Vector2i& start, const sf::Vector2i& end, const uint32_t intensity, bool force, const int64_t heat, const unsigned char collect_percent = 10, bool turnToAsh = false);
    
    
    void mine(sf::Vector2i p, const uint32_t intensity, const int percent_gain = 0);

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

    bool generate();
    bool generateVegetation();

    void loadProcGenData(const std::string& path) {
        procGen.loadHeightMap(path);
        procGen.loadBiomeData(path);
    }

    void save() {
        const std::string created_folder = StorageSettings::save_path + Globals::exitSaveName;

        if (!std::filesystem::create_directories(created_folder + "/data")) {
            prnerr("Could not create folder for save! Path is ", created_folder);
            return;
        }


        loginf("Created save folder folder located in ", created_folder, ".");

        std::vector<std::future<void>> futures;

        for (int y = 0; y < chunks_y; y++) {
            for (int x = 0; x < chunks_x; x++) {
                const std::string filename = std::to_string(x) + "_" + std::to_string(y);
                const std::string fullpath = created_folder + "/" + filename;

                futures.emplace_back(std::async(std::launch::deferred, [fullpath, filename, this, x, y]() {
                    this->getChunkIndexer().getChunkAt(x, y).getImage().saveToFile(fullpath + ".png");
                    return;
                }));

                prndd("Chunk saved.");
            }
        }

        prndd("Finished saving chunks, waiting for threads to finish...");

        for (auto& future : futures) {
            future.wait();
        }

        prndd("Preparing saving height map...");

        const std::string hm_path = created_folder + "/data/heightMap.dat";

        std::fstream heightMapFile;
        heightMapFile.open(created_folder + "/data/heightMap.dat", std::fstream::out);

        for(int i = 0; i < worldSize::world_sx - 1; i++) {
            heightMapFile << procGen.getHeightOnMap(i) << std::endl;
        }

        heightMapFile.close();

        std::fstream biomeDataFile;
        biomeDataFile.open(created_folder + "/data/biomeData.dat", std::fstream::out);

        auto biomeData = procGen.getBiomeData();
        for(const auto &biome : biomeData) {
            biomeDataFile << biome->getName() << std::endl;
        }

        biomeDataFile.close();

        prndd("All saving threads finished.");
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

        if(chIndexer.getVoxelAt(pos1.x, pos1.y).value == voxelValue) 
            return pos1;

        if(chIndexer.getVoxelAt(pos2.x, pos2.y).value == voxelValue) 
            return pos2;

        if(chIndexer.getVoxelAt(pos3.x, pos3.y).value == voxelValue) 
            return pos3;

        if(chIndexer.getVoxelAt(pos4.x, pos4.y).value == voxelValue) 
            return pos4;

        return sf::Vector2i(0, 0);
    }

    const Voxel getHandleVoxel(const sf::Color &px, sf::Vector2i p, bool addVoxelsToArr = false) {
        Voxel vox = Voxel();
        vox.value = px.a != 0;
        if(vox.value == 0) return vox;

        if(px == elm::getInfoFromType(VoxelValues::CARBON).color) {
            vox.value = 2;
            vox.strenght = 8;
        } else if(px == elm::getInfoFromType(VoxelValues::LITHIUM).color) {
            vox.value = 3;
            vox.strenght = 2;
            
            if(addVoxelsToArr) reactiveVoxels.push_back(p);
        } else if(px == elm::getInfoFromType(VoxelValues::MAGNESIUM).color) {
            vox.value = 4;
            vox.strenght = 10;

            if(addVoxelsToArr) addElement(p.x, p.y, std::make_shared<Flammable>(p.x, p.y));
            
        } else if(px == elm::getInfoFromType(VoxelValues::WOOD).color) {
            vox.value = 4;
            vox.strenght = 10;

            if(addVoxelsToArr) addElement(p.x, p.y, std::make_shared<Wood>(p.x, p.y));
            
        } else if(px == elm::getInfoFromType(VoxelValues::SODIUM).color) {
            vox.value = 5;
            vox.strenght = 1;

            if(addVoxelsToArr) reactiveVoxels.push_back(p);
        } else if(px == elm::getInfoFromType(VoxelValues::ALUMINIUM).color) {
            vox.value = 6;
            vox.strenght = 5;
        } else if(px == elm::getInfoFromType(VoxelValues::SILICON).color) {
            vox.value = 7;
            vox.strenght = 6;
        } else if(px == elm::getInfoFromType(VoxelValues::COPPER).color) {
            vox.value = 8;
            vox.strenght = 10;
        } else if(px == elm::getInfoFromType(VoxelValues::TITANIUM).color) {
            vox.value = 9;
            vox.strenght = 100;
        } else if(px == elm::getInfoFromType(VoxelValues::LEAD).color) {
            vox.value = 10;
            vox.strenght = 3;
        } else if(px == elm::getInfoFromType(VoxelValues::WATER).color) {
            vox.value = VoxelValues::WATER;
            vox.strenght = 20;

            if(addVoxelsToArr) addElement(p.x, p.y, std::make_shared<Water>(p.x, p.y));
        } else if(px == elm::getInfoFromType(VoxelValues::BLOOD).color) {
            vox.value = VoxelValues::BLOOD;
            vox.strenght = 2;

            if(addVoxelsToArr) addElement(p.x, p.y, std::make_shared<Blood>(p.x, p.y, sf::Vector2i(0,0)));
        } else if(px == elm::getInfoFromType(VoxelValues::ACID).color) {
            vox.value = VoxelValues::ACID;
            vox.strenght = 2;

            if(addVoxelsToArr) addElement(p.x, p.y, std::make_shared<Acid>(p.x, p.y));
        }else if(px == elm::getInfoFromType(VoxelValues::NITROGLYCERIN).color) {
            vox.value = VoxelValues::NITROGLYCERIN;
            vox.strenght = 2;
            if(addVoxelsToArr) addElement(p.x, p.y, std::make_shared<Nitroglycerin>(p.x, p.y));
        } else if(px == elm::getInfoFromType(VoxelValues::CHLORINE).color) {
            // TODO make workd!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!111
            //vox.value = elm::ValChlorine;
            //vox.strenght = 2;

            //if(addVoxelsToArr) addElement(p.x, p.y, std::make_shared<Chlorine>(p.x, p.y));

            vox.value = VoxelValues::CARBON;
            vox.strenght = 1;

            std::shared_ptr<ElectricComponent> led = std::make_shared<Led>();
            led->x = p.x;
            led->y = p.y;
            
            if(addVoxelsToArr) simulationManager.addElementToES(led);
        } else if(px == elm::getInfoFromType(VoxelValues::URANIUM235).color) {
            vox.value = VoxelValues::URANIUM235;
            vox.strenght = 2;

            if(addVoxelsToArr) addRadioactiveElement(p.x, p.y, std::make_shared<Uranium235>(p.x, p.y));
        }

        else if(px == elm::getInfoFromType(VoxelValues::RADIUM226).color) {
            vox.value = VoxelValues::RADIUM226;
            vox.strenght = 2;

            if(addVoxelsToArr) addRadioactiveElement(p.x, p.y, std::make_shared<Radium226>(p.x, p.y));
        }

        else if(px == elm::getInfoFromType(VoxelValues::SAND).color) {
            vox.value = VoxelValues::SAND;
            vox.strenght = 250;

            if(addVoxelsToArr) addElement(p.x, p.y, std::make_shared<Sand>(p.x, p.y));
        }

        else if(px == elm::getInfoFromType(VoxelValues::SNOW).color) {
            vox.value = VoxelValues::SNOW;
            vox.strenght = 1;
            if(addVoxelsToArr) addElement(p.x, p.y, std::make_shared<Snow>(p.x, p.y));
        }

        else if(px == elm::getInfoFromType(VoxelValues::MIRROR).color) {
            vox.value = VoxelValues::MIRROR;
            vox.strenght = 255;
        }

        else if(px == elm::getInfoFromType(VoxelValues::OSCILLATOR).color) {
            vox.value = VoxelValues::OSCILLATOR;
            vox.strenght = 1;
            
            std::shared_ptr<ElectricComponent> oscillator = std::make_shared<Oscillator>();
            oscillator->x = p.x;
            oscillator->y = p.y;
            
            if(addVoxelsToArr) simulationManager.addElementToES(oscillator);
        }

        else if(px == elm::getInfoFromType(VoxelValues::TRANSISTORDON).color) {
            vox.value = VoxelValues::TRANSISTORDON;
            vox.strenght = 1;
            
            std::shared_ptr<ElectricComponent> transistor = std::make_shared<TransistorDON>();
            transistor->x = p.x;
            transistor->y = p.y;
            
            if(addVoxelsToArr) simulationManager.addElementToES(transistor);
        }

        else if(px == elm::getInfoFromType(VoxelValues::TRANSISTORDOFF).color) {
            vox.value = VoxelValues::TRANSISTORDOFF;
            vox.strenght = 1;
            
            std::shared_ptr<ElectricComponent> transistor = std::make_shared<TransistorDOFF>();
            transistor->x = p.x;
            transistor->y = p.y;
            
            if(addVoxelsToArr) simulationManager.addElementToES(transistor);
        }

        else if(px == elm::getInfoFromType(VoxelValues::SWITCH).color) {
            vox.value = VoxelValues::SWITCH;
            vox.strenght = 1;
            
            std::shared_ptr<ElectricComponent> switch_ = std::make_shared<Switch>(p.x, p.y);
            switch_->x = p.x;
            switch_->y = p.y;
            
            if(addVoxelsToArr) simulationManager.addElementToES(switch_);
        }

        else if(px == elm::getInfoFromType(VoxelValues::BUTANE).color) {
            vox.value = VoxelValues::BUTANE;
            vox.strenght = 2;

            if(addVoxelsToArr) addElement(p.x, p.y, std::make_shared<Butane>(p.x, p.y));
        }

        else if(px == elm::getInfoFromType(VoxelValues::MANGANESEHEPTOXIDE).color) {
            vox.value = VoxelValues::MANGANESEHEPTOXIDE;
            vox.strenght = 2;

            if(addVoxelsToArr) addElement(p.x, p.y, std::make_shared<ManganeseHeptoxide>(p.x, p.y));
        }

        return vox;
    }

    void addElement(int x, int y, const std::shared_ptr<Element> element) {
        chIndexer.boundGetChunkAt(chIndexer.getChunkFromPos(x,y).x, chIndexer.getChunkFromPos(x,y).y).needs_update = true;

        chIndexer.boundGetChunkAt(chIndexer.getChunkFromPos(x, y).x, chIndexer.getChunkFromPos(x, y).y).elements.push_back(element);
    }

    void addRadioactiveElement(int x, int y, const std::shared_ptr<RadioactiveElement> element) {
        chIndexer.boundGetChunkAt(chIndexer.getChunkFromPos(x, y).x, chIndexer.getChunkFromPos(x, y).y).radioactive_elements.push_back(element);
    }

    void valueFromColor() {

    }

    ChunkIndexer &getChunkIndexer() {
        return chIndexer;
    }

    void unloadAll() {
        ChunkBounds bounds = ChunkBounds(0, 0, chunks_x, chunks_y);
        unloadArea(bounds);
    }

    void unloadArea(ChunkBounds bounds) {
        for(int64_t y = bounds.getArea().startY; y < bounds.getArea().endY; y++) {
            for(int64_t x = bounds.getArea().startX; x < bounds.getArea().endX; x++) {
                chIndexer.getChunkAt(x, y).unLoad();
            }
        }

        // Note: Only on linux
        malloc_trim(0);
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

    void removeMaterialsOfImage(sf::Image &img) {
        for(unsigned int y = 0; y < img.getSize().y; y++) {
            for(unsigned int x = 0; x < img.getSize().x; x++) {
                const sf::Color pixel = img.getPixel(x, y);

                const int voxel_value = getHandleVoxel(pixel, sf::Vector2i(x, y)).value;
                chIndexer.materialpack.addElementOfType(voxel_value, -1);
            }
        }
    }

    void build_image(const sf::Vector2i&, const sf::Image&, std::list<VoxelGroup>*, const sf::Vector2f velocity = sf::Vector2f(0.f, 0.f), bool hasCollisions = true);

    const sf::FloatRect &getUpdateArea() {
        return update_area;
    }

public:
    ProcGenerate procGen;
    
    std::vector<ExplosionInfo> explosion_points;
    std::vector<sf::Vector2i> updateChunks;

private:

    sf::FloatRect update_area;

    ChunkIndexer chIndexer;

    std::vector<sf::Vector2i> mergeChunks;

    std::list<sf::Vector2i> voxelsInNeedOfUpdate;
    std::list<sf::Vector2i> reactiveVoxels;

    ParticleSimulation particleSimulation;
    SimulationManager simulationManager;
    FireEffectManager fireEffectManager;
};