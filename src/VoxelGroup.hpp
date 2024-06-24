#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <math.h>
#include <thread>
#include <optional>
#include <list>
#include <filesystem>
#include <iostream>

#include "math.hpp"
#include "Voxel.hpp"

#include "ExplosionInfo.hpp"
#include "common.hpp"
#include "Elements.hpp"
#include "MaterialPack.hpp"
#include "PhysicsComponent.hpp"
#include "Collider.hpp"
#include "Dispawner.hpp"
#include "Raycast.hpp"
#include "RectangleRigidbody.hpp"
#include "Globals.hpp"
#include "Controls.hpp"
#include "VoxelGroupPropeties/VoxelGroupPropeties.hpp"
#include "VoxelGroupIDGenerator.hpp"
#include "Graphics/VertexSprite.hpp"

#include <list>
#include <memory>

// We cant include voxelmanager, becouse that would create an infinite preprocessor loop
class VoxelManager;

class VoxelGroup : public DispawnableVoxelObject {
public:

    VoxelGroup() : DispawnableVoxelObject() {

    }

    bool loadFromStorage(const std::string &path, const int id = 0) {
        voxelGroupID = id;

        std::ifstream infoFileStream;
        infoFileStream.open(path + "/data.rdata", std::ifstream::in);

        if(!infoFileStream.is_open()) {
            prnerr("Could not open info file stream when loading from storage! Path is ", path + "/data.rdata");
        }

        float left   = 1.f;
        float top    = 1.f;
        float width  = 1.f;
        float height = 1.f;

        infoFileStream >> left >> top >> width >> height;

        if (infoFileStream.fail()) {
            prnerr("Error reading values from file", "!");
        }

        infoFileStream.close();

        if(!img.loadFromFile(path + "/img.png")) {
            prnerr("Could not load image from ", path);
            img.create(16, 16, sf::Color::Magenta);
        }

        if(img.getSize().x != width || img.getSize().y != height) {
            prnerr("Loaded image size does not match info.rdata size (lines 3-4)", "!");
        }

        sf::FloatRect rect;

        world_sx = img.getSize().x;
        world_sy = img.getSize().y;

        tex.create(world_sx,world_sy);

        for(unsigned int i = 0; i < img.getSize().y; i++)
        {
            std::vector <Voxel> voxRow;
            for(unsigned int j = 0; j < img.getSize().x; j++)
            {
                voxRow.push_back(Voxel());
            }
            grid.push_back(voxRow);
        }

        for (uint64_t y = 0; y < world_sy; y++) {
            for (uint64_t x = 0; x < world_sx; x++) {
                const sf::Color px = img.getPixel(x,y);
                getVoxelAt(x,y) = getValueFromCol(px, sf::Vector2i(x,y));
            }
        }

        tex.update(img);
        vertSpr.load(tex);

        setPosition(sf::Vector2f(left, top));
        return true;
    }

    void save() {
        if(!VoxelGroupIDGenerator::checkID(voxelGroupID)) {
            prnerr("Voxel group ID was set to 0 while trying to save", "!");         
            return;
        }

        const std::string created_folder = StorageSettings::save_path + Globals::exitSaveName + "/voxelGroups/" + std::to_string(voxelGroupID);
        const std::string infoFile = created_folder + "/data.rdata";

        if (!std::filesystem::create_directories(created_folder)) {
            prnerr("Could not create folder for voxel groups! Path is ", created_folder);
            return;
        }

        loginf("Voxel group save folder created in ", created_folder, ".");

        img.saveToFile(created_folder + "/img.png");

        std::fstream infoFileStream;
        infoFileStream.open(infoFile, std::fstream::out);

        if(infoFileStream.bad()) {
            prnerr("Could not create info file for voxel groups! Path is ", created_folder);
            infoFileStream.close();
            return;
        }

        infoFileStream << vertSpr.getVertex(0).position.x   << '\n';
        infoFileStream << vertSpr.getVertex(0).position.y   << '\n';
        infoFileStream << tex.getSize().x << '\n';
        infoFileStream << tex.getSize().y << '\n';

        infoFileStream.close();
        destroy();
    }
    
    std::pair<bool, sf::FloatRect> getOvelapWithRect(const sf::FloatRect &collider);
    std::pair<bool, sf::FloatRect> getOvelapWithRectY(const sf::FloatRect &collider);

    int load(const sf::Image &copy_img);

    Voxel &getVoxelAt (const uint64_t x, const uint64_t y) {
        return grid[y][x];
    }

    void setPosition(const sf::Vector2f& pos) {
        sf::Sprite boundSprite;
        boundSprite.setPosition(pos.x, pos.y);
        boundSprite.setTexture(tex);

        rigidBody.loadFromRectangle(boundSprite.getGlobalBounds());
    }

    const sf::Vector2f& getPosition() {
        return vertSpr.getVertex(0).position;
    }

    void setVelocity(const sf::Vector2f& vel) {
        rigidBody.setVelocity(vel);
    }

    sf::FloatRect getCollider() {
        if(destroyed) return sf::FloatRect(-1000,-1000,0,0);
        return sf::FloatRect(vertSpr.getVertex(0).position, sf::Vector2f(tex.getSize()));
    }

    void clearVoxelAt(const uint64_t x, const uint64_t y) {
        getVoxelAt(x,y).value = 0; 
        getVoxelAt(x,y).temp = 0; 
        setImagePixelAt(x,y,sf::Color(0,0,0,0));
    }

    void damageVoxelAt(const uint64_t x, const uint64_t y) {
        uint8_t &strenght = getVoxelAt(x,y).strenght;
        --strenght;
        if(getVoxelAt(x,y).strenght <= 0) { 
            clearVoxelAt(x,y);
        }
    }

    void heatVoxelAt(const int64_t x, const int64_t y, int64_t temp);
    void render(sf::RenderTarget&, const sf::Vector2f &center);
    void update(ChunkIndexer &vx, const float);
    void hole(const sf::Vector2i &pos, const uint32_t &intensity, bool force, const int64_t heat);

    sf::Color getImagePixelAt(const uint64_t x, const uint64_t y) {
        return img.getPixel(x,y);
    }

    void setImagePixelAt(const uint64_t x, const uint64_t y, const sf::Color& color) {
        img.setPixel(x,y,color);
    }

    template<class T>

    void destroyPart(T& main_world) {
        if(getDestroyed()) return;
        
        for (uint64_t y = 0; y < world_sy;y++) {
            for (uint64_t x = 0; x < world_sx;x++) {
                if(getVoxelAt(x,y).value == VoxelValues::LITHIUM) {
                    main_world.holeRayCast(
                        sf::Vector2i(vertSpr.getVertex(0).position + sf::Vector2f(x,y)), elm::lithiumExplosion, true, 100);
                }
                damageVoxelAt(x,y);
            }
        }
        destroy();
    }

    void setPropeties(const VoxelGroupPropeties& props) {
        propeties = props;
    }

    VoxelGroupPropeties& getPropeties() {
        return propeties;
    }

    void dispawn() {
        destroy();
    }

    std::vector <ExplosionInfo> explosion_points;
    std::vector<sf::Vector2i> updateChunks;
    sf::Image img;
private:

    int voxelGroupID = 0;

    void destroy() {
        grid.clear();
        rects.clear();
        setDestroyed(true);
    }

    RectangleRigidbody rigidBody;
    VoxelGroupPropeties propeties {};

    std::vector<std::vector<Voxel>> grid;
    std::vector<Collider> rects;
    sf::Texture tex;
    VertexSprite vertSpr;

    uint64_t world_sx;
    uint64_t world_sy;
};