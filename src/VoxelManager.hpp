#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <math.h>
#include <thread>
#include "math.hpp"
#include "Voxel.hpp"

#include "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/src/ExplosionInfo.hpp"
#include "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/src/common.hpp"
#include "Elements.hpp"
#include <list>



class VoxelManager {
public:

    const int gx = 128; 
    const int gy = 128;

    int chunks_x = 16;
    int chunks_y = 16;

    VoxelManager() : grid(chunks_y + 1, std::vector<My2DArray>(chunks_x + 1)) {
        prndd("Populating array");
        for (int y = 0; y < chunks_y; ++y)
        {
            for (int x = 0; x < chunks_x; ++x)
            {
                grid[x][y] = My2DArray();
            }
        }
    }
    std::pair<bool,float> checkCollisionsWith(const sf::FloatRect &collider);
    std::pair<bool,float> checkCollisionsWithInv(const sf::FloatRect &collider);
    std::pair<bool,float> checkCollisionsWithRight(const sf::FloatRect &collider);
    std::pair<bool,float> checkCollisionsWithLeft(const sf::FloatRect &collider);

    int load(std::string);

    Voxel &getVoxelAt (const uint64_t x, const uint64_t y) {
        return grid.at(x/gx).at(y/gy).arr[x%gx][y%gy];
    }

    void clearVoxelAt(const uint64_t x, const uint64_t y) {
        getVoxelAt(x,y).value = 0; 
        img.setPixel(x,y,sf::Color(0,0,0,0));
    }

    void damageVoxelAt(const uint64_t x, const uint64_t y) {
        getVoxelAt(x,y).strenght--;
        if(getVoxelAt(x,y).strenght <= 0) clearVoxelAt(x,y);
    }

    void heatVoxelAt(const uint64_t x, const uint64_t y, const float temp);



    void render(sf::RenderTarget&);
    void resetUsedFlag();
    void update();
    void merge(bool useChunks = false);
    void hole(const sf::Vector2i &pos, const uint32_t &intensity, bool force, const uint32_t heat);

    const Voxel getValueFromCol(const sf::Color &px, sf::Vector2i p);

    void save() {
        img.saveToFile("res/saves/" + std::to_string(time(0)) + ".png");
    }

    void build_image(const sf::Vector2i&, const sf::Image&);

    void copy() {
        rects_copy = rects;
    }

    void lock() {
        thread_lock = true;
    }

    void release() {
        thread_lock = false;
    }

    bool locked() {
        return thread_lock;
    }

    std::vector <ExplosionInfo> explosion_points;
    std::vector<sf::Vector2i> updateChunks;

private:

    std::list<sf::Vector2i> voxelsInNeedOfUpdate;
    std::list<sf::Vector2i> recativeVoxels;


    bool thread_lock = false;

    sf::Shader shader;

    std::thread explode;
    std::vector <sf::Sprite> rects;
    std::vector <sf::Sprite> rects_copy;


    struct My2DArray {
        Voxel arr[128][128];
    };


    std::vector<std::vector<My2DArray>> grid;

    bool debug = false;

    uint64_t world_sx;
    uint64_t world_sy;


    sf::Texture world_tx;
    sf::Image img;
    sf::Sprite world_spr;

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