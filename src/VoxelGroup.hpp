#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <math.h>
#include <thread>
#include <optional>
#include <list>

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

#include <list>
#include <memory>

// We cant include voxelmanager, becouse that would create an infinite preprocessor loop
class VoxelManager;

class VoxelGroup : public DispawnableVoxelObject {

public:

    VoxelGroup() : DispawnableVoxelObject() {

    }
    
    std::pair<bool, sf::FloatRect> getOvelapWithRect(const sf::FloatRect &collider);
    std::pair<bool, sf::FloatRect> getOvelapWithRectY(const sf::FloatRect &collider);

    int load(const sf::Image &copy_img);

    Voxel &getVoxelAt (const uint64_t x, const uint64_t y) {
        return grid[y][x];
    }

    void setPosition(const sf::Vector2f& pos) {
        spr.setPosition(pos);
    }

    const sf::Vector2f& getPosition() const {
        return spr.getPosition();
    }

    void setVelocity(const sf::Vector2f& vel) {
        rigidBody.setVelocityInAllPoints(vel);
    }

    sf::FloatRect getCollider() {
        if(destroyed) return sf::FloatRect(-1000,-1000,0,0);
        return spr.getGlobalBounds();
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
    void resetUsedFlag();
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
                        sf::Vector2i(spr.getPosition() + sf::Vector2f(x,y)), elm::lithiumExplosion, true, 100);
                }
                damageVoxelAt(x,y);
            }
        }
        destroy();
    }

    void dispawn() {
        destroy();
    }

    std::vector <ExplosionInfo> explosion_points;
    std::vector<sf::Vector2i> updateChunks;
    sf::Image img;
private:

    void destroy() {
        grid.clear();
        rects.clear();
        setDestroyed(true);
    }

    RectangleRigidbody rigidBody;

    std::vector<std::vector<Voxel>> grid;
    std::vector<Collider> rects;
    sf::Texture tex;
    sf::Sprite spr;

    uint64_t world_sx;
    uint64_t world_sy;
};