#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <math.h>
#include <thread>
#include <optional>

#include "math.hpp"
#include "Voxel.hpp"

#include "ExplosionInfo.hpp"
#include "common.hpp"
#include "Elements.hpp"
#include "MaterialPack.hpp"
#include "PhysicsComponent.hpp"
#include <list>
#include <memory>

class VoxelGroup {
public:

    VoxelGroup() {

    }
    std::pair<bool, sf::FloatRect> getOvelapWithRect(const sf::FloatRect &collider);
    std::pair<bool, sf::FloatRect> getOvelapWithRectY(const sf::FloatRect &collider);

    int load(std::string, bool);

    Voxel &getVoxelAt (const uint64_t x, const uint64_t y) {
        return grid[y][x];
    }

    void clearVoxelAt(const uint64_t x, const uint64_t y) {
        getVoxelAt(x,y).value = 0; 
        setImagePixelAt(x,y,sf::Color(0,0,0,0));
    }

    void damageVoxelAt(const uint64_t x, const uint64_t y) {
        uint8_t &strenght = getVoxelAt(x,y).strenght;
        --strenght;
        if(getVoxelAt(x,y).strenght <= 0) { 
            clearVoxelAt(x,y);
        }
    }

    void heatVoxelAt(const uint64_t x, const uint64_t y, int64_t temp);
    void render(sf::RenderTarget&, const sf::Vector2f &center);
    void resetUsedFlag();
    void update();
    void merge();
    void hole(const sf::Vector2i &pos, const uint32_t &intensity, bool force, const int64_t heat);

    const Voxel getValueFromCol(const sf::Color &px, sf::Vector2i p);

    void save() {
        // TODO : make it save from the chunk images
        //img.saveToFile("res/saves/" + std::to_string(time(0)) + ".png");
    }

    sf::Color getImagePixelAt(const uint64_t x, const uint64_t y) {
        return img.getPixel(x,y);
    }

    void setImagePixelAt(const uint64_t x, const uint64_t y, const sf::Color& color) {
        img.setPixel(x,y,color);
    }

    std::vector <ExplosionInfo> explosion_points;
    std::vector<sf::Vector2i> updateChunks;

private:

    PhysicsComponent physicsComponent;

    std::list<sf::Vector2i> voxelsInNeedOfUpdate;
    std::list<sf::Vector2i> mergeChunks;

    sf::Shader shader; 
    std::vector<std::vector<Voxel>> grid;
    std::vector<sf::FloatRect> rects;
    sf::Image img;
    sf::Texture tex;
    sf::Sprite spr;

    bool debug = false;

    uint64_t world_sx;
    uint64_t world_sy;

    const char * shader_frag = 
    R"( 
        uniform sampler2D texture;

        void main()
        {
            
            // lookup the pixel in the texture
            vec4 pixel = vec4(0.0, 1.0, 0.0, 1.0);

            // multiply it by the color
            gl_FragColor = pixel;
        }
    )";

    const char * shader_vert = 
    R"( 
        void main()
        {
            // transform the vertex position
            gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

            // transform the texture coordinates
            gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

            // forward the vertex color
            gl_FrontColor = gl_Color;
        }
    )";

};