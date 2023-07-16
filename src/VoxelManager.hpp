#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <math.h>
#include <thread>
#include "math.hpp"
#include "Voxel.hpp"

#include "ExplosionInfo.hpp"
#include "common.hpp"
#include "Elements.hpp"
#include "Chunk.hpp"
#include <list>



class VoxelManager {
public:

    VoxelManager() : grid(chunks_y + 1, std::vector<Chunk>(chunks_x + 1)) {
        prndd("Populating array");
        for (int y = 0; y < chunks_y; ++y)
        {
            for (int x = 0; x < chunks_x; ++x)
            {
                grid[x][y] = Chunk();
            }
        }
    }
    std::pair<bool, sf::FloatRect> getOvelapWithRect(const sf::FloatRect &collider);

    int load(std::string);

    Voxel &getVoxelAt (const uint64_t x, const uint64_t y) {
        return grid.at(x/Chunk::sizeX).at(y/Chunk::sizeY).requestAccess()[x%Chunk::sizeX][y%Chunk::sizeY];
    }

    void clearVoxelAt(const uint64_t x, const uint64_t y) {
        getVoxelAt(x,y).value = 0; 
        img.setPixel(x,y,sf::Color(0,0,0,0));
    }

    void damageVoxelAt(const uint64_t x, const uint64_t y) {
        long &strenght = getVoxelAt(x,y).strenght;
        --strenght;
        if(getVoxelAt(x,y).strenght <= 0) clearVoxelAt(x,y);
    }

    void heatVoxelAt(const uint64_t x, const uint64_t y, int64_t temp);
    void render(sf::RenderTarget&, const sf::Vector2f &center);
    void resetUsedFlag();
    void update();
    void merge();
    void hole(const sf::Vector2i &pos, const uint32_t &intensity, bool force, const int64_t heat);

    void mergeChunkBounds(const ChunkBounds &bounds) {
        for(uint32_t y = bounds.getArea().startY; y < bounds.getArea().endY; y++) {
        for(uint32_t x = bounds.getArea().startX; x < bounds.getArea().endX; x++) {
                mergeChunks.push_back(sf::Vector2i(x, y));
            }
        }
        merge();
    }

    const Voxel getValueFromCol(const sf::Color &px, sf::Vector2i p);

    void save() {
        img.saveToFile("res/saves/" + std::to_string(time(0)) + ".png");
    }

    void build_image(const sf::Vector2i&, const sf::Image&);

    std::vector <ExplosionInfo> explosion_points;
    std::vector<sf::Vector2i> updateChunks;

private:

    std::list<sf::Vector2i> voxelsInNeedOfUpdate;
    std::list<sf::Vector2i> recativeVoxels;
    std::list<sf::Vector2i> mergeChunks;

    sf::Shader shader; 
    std::vector<std::vector<Chunk>> grid;

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