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
#include "Chunk.hpp"
#include "MaterialPack.hpp"
#include "VoxelGroup.hpp"
#include "ProceduralGeneration.hpp"
#include <list>

class VoxelManager {
public:

    VoxelManager() : chIndexer() {

    }
    std::pair<bool, sf::FloatRect> getOvelapWithRect(const sf::FloatRect &collider);
    std::pair<bool, sf::FloatRect> getOvelapWithRectY(const sf::FloatRect &collider);

    int load(std::string, bool);

    void initVoxelMap() {
        for (int y = 0;y < world_sy;y++) {
            for (int x = 0;x < world_sx;x++) {
                const sf::Color px = getImagePixelAt(x,y);
                getVoxelAt(x,y) = getValueFromCol(px, sf::Vector2i(x,y));
            }
        }
    }

    void clearVoxelAt(const uint64_t x, const uint64_t y) {
        getVoxelAt(x,y).value = 0; 
        setImagePixelAt(x,y,sf::Color(0,0,0,0));
    }

    void damageVoxelAt(const uint64_t x, const uint64_t y) {
        uint8_t &strenght = getVoxelAt(x,y).strenght;
        --strenght;
        if(getVoxelAt(x,y).strenght <= 0) { 
            // I know this is terrible
            if(getVoxelAt(x, y).value == elm::ValCarbon) materialpack.carbon += 1;
            else if(getVoxelAt(x, y).value == elm::ValLithium) materialpack.lithium += 1;
            else if(getVoxelAt(x, y).value == elm::ValMagnesium) materialpack.magnesium += 1;
            else if(getVoxelAt(x, y).value == elm::ValSodium) materialpack.sodium += 1;
            else if(getVoxelAt(x, y).value == elm::ValAluminium) materialpack.aluminium += 1;
            else if(getVoxelAt(x, y).value == elm::ValSilicon) materialpack.silicon += 1;
            else if(getVoxelAt(x, y).value == elm::ValCopper) materialpack.copper += 1;
            else if(getVoxelAt(x, y).value == elm::ValTitanium) materialpack.titanium += 1;
            else if(getVoxelAt(x, y).value == elm::ValLead) materialpack.lead += 1;

            clearVoxelAt(x,y);
        }
    }

    void heatVoxelAt(const uint64_t x, const uint64_t y, int64_t temp);
    void render(sf::RenderTarget&, const sf::Vector2f &center);
    void resetUsedFlag();
    void update();
    void merge();
    void hole(const sf::Vector2i &pos, const uint32_t &intensity, bool force, const int64_t heat);

    bool generate();
    bool generateVegetation();


    void mergeChunkBounds(const ChunkBounds &bounds) {
        for(int32_t y = bounds.getArea().startY; y < bounds.getArea().endY; y++) {
        for(int32_t x = bounds.getArea().startX; x < bounds.getArea().endX; x++) {
                mergeChunks.push_back(sf::Vector2i(x, y));
            }
        }
        merge();
    }

    void save() {
        // TODO : make it save from the chunk images
        //img.saveToFile("res/saves/" + std::to_string(time(0)) + ".png");
    }

    Voxel &getVoxelAt (const uint64_t x, const uint64_t y) {
        return chIndexer.getChunkAt(x/Chunk::sizeX, y/Chunk::sizeY).requestAccess()[x%Chunk::sizeX][y%Chunk::sizeY];
    }
    
    const sf::Color getImagePixelAt(const uint64_t x, const uint64_t y) {
        return chIndexer.getChunkAt(x/Chunk::sizeX, y/Chunk::sizeY).requestImageAccess().getPixel(x%Chunk::sizeX, y%Chunk::sizeY);
    }

    void setImagePixelAt(const uint64_t x, const uint64_t y, const sf::Color& color) {
        chIndexer.getChunkAt(x/Chunk::sizeX, y/Chunk::sizeY).requestImageAccess().setPixel(x%Chunk::sizeX, y%Chunk::sizeY, color);
    }

    MaterialPack &getReceivedMaterials() {
        return materialpack;
    }

    void build_image(const sf::Vector2i&, const sf::Image&, std::vector<VoxelGroup>*, float angle = 0.f, float mag = 0.f);

    std::vector <ExplosionInfo> explosion_points;
    std::vector<sf::Vector2i> updateChunks;

private:

    ProcGenerate procGen;

    MaterialPack materialpack;

    ChunkIndexer chIndexer;

    std::list<sf::Vector2i> voxelsInNeedOfUpdate;
    std::list<sf::Vector2i> mergeChunks;

    sf::Shader shader; 
    std::vector<float> hmap1D;

    bool debug = false;

    int64_t world_sx;
    int64_t world_sy;

    int64_t world_snegx;
    int64_t world_snegy;

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