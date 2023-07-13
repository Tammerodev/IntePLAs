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

    int load();

    Voxel &getVoxelAt (const uint64_t x, const uint64_t y) {
        return grid.at(x/gx).at(y/gy).arr[x%gx][y%gy];
    }

    void render(sf::RenderTarget&);
    void resetUsedFlag();
    void update();
    void merge(bool useChunks = false);
    void hole(const sf::Vector2i &pos, const uint32_t &intensity, bool rec = true);

    void getValueFromCol(const sf::Color &px, sf::Vector2i pos) {
        int x = pos.x;
        int y = pos.y;

                        if(px.r == 34 && px.g == 196 && px.b == 34) getVoxelAt(x,y).value = 2;       // <-- Green explosibe thing
        else if(px.r == 204 && px.g == 223 && px.b == 223 ) getVoxelAt(x,y).value = 3;   // White explosive
        else if(px.r == 17 && px.g == 17 && px.b == 17 ) { getVoxelAt(x,y).value = 4; getVoxelAt(x,y).strenght = 5; }  // Stronk
        else if(px.r == 36 && px.g == 135 && px.b == 240) getVoxelAt(x,y).value = 5;       // <-- Nuclear bomb 36,135,240
        else if(px.r == 90 && px.g == 110 && px.b == 255) {getVoxelAt(x,y).value = 6; getVoxelAt(x,y).isFalling = true;}       // <-- Nuclear bomb 36,135,240
    }

    void build_image(const sf::Vector2i&, const sf::Image&);
    void build_circle(const sf::Vector2i &p, const uint32_t &intensity) {
        int yexcept = p.y - intensity;
        int xexcept = p.x - intensity;

        if(yexcept < 0) yexcept = 0;
        if(xexcept < 0) xexcept = 0;

        for (int y = yexcept;y < p.y + intensity;y++) {
            if(p.y > world_sy) break;

            for (int x = xexcept;x < p.x + intensity;x++) {
                if(x >world_sx) break;
                if(math::isqrt((p.x - x)*(p.x- x) + ((p.y - y)*(p.y - y))) < intensity) {
                    getVoxelAt(x,y).value = 1;
                    img.setPixel(x,y,sf::Color(255,255,255,255));
                }
            }
        }
        merge();
    }

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