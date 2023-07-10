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



class VoxelManager {
public:

    const int gx = 128; 
    const int gy = 128;


    int chunks_x = 16;
    int chunks_y = 16;


    VoxelManager() : grid(chunks_y* 2, std::vector<My2DArray>(chunks_x * 2)) {
        prndd("Populating array");
        for (int y = 0; y < chunks_y; ++y)
        {
            for (int x = 0; x < chunks_x; ++x)
            {
                grid[y][x] = My2DArray();
                prndd("Added 2D array to vector");
            }
        }
    }
    std::pair<bool,float> checkCollisionsWith(const sf::FloatRect &collider);
    std::pair<bool,float> checkCollisionsWithInv(const sf::FloatRect &collider);
    std::pair<bool,float> checkCollisionsWithRight(const sf::FloatRect &collider);
    std::pair<bool,float> checkCollisionsWithLeft(const sf::FloatRect &collider);

    int load();

    Voxel &getVoxelAt (const uint64_t x, const uint64_t y) {
        return grid.at(y/gy).at(x/gx).arr[(x / gx)*16 - (x % gx)][(y / gy) - (y % gy)];
    }

    void render(sf::RenderTarget&);
    void resetUsedFlag();
    void update();
    void merge();
    void hole(const sf::Vector2i &pos, const uint32_t &intensity, bool rec = true);

    std::vector <ExplosionInfo> explosion_points;

private:

    sf::Shader shader;

    std::thread explode;
    std::vector <sf::Sprite> rects;

    struct My2DArray {
        Voxel arr[128][128];
    };


    std::vector<std::vector<My2DArray>> grid;

    bool debug = false;
    int indexX = 0;
    int y = 0;
    int x = 0;

    uint64_t world_sx;
    uint64_t world_sy;


    sf::Texture world_tx;
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