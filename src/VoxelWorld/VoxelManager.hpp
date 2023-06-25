#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <math.h>
#include <thread>
#include "math.hpp"
#include "Voxel.hpp"

constexpr int gl = 2048; 
constexpr int gw = 2048;

class VoxelManager {
public:
    VoxelManager() {

    }
    std::pair<bool,float> checkCollisionsWith(const sf::FloatRect &collider);
    std::pair<bool,float> checkCollisionsWithInv(const sf::FloatRect &collider);
    std::pair<bool,float> checkCollisionsWithRight(const sf::FloatRect &collider);
    std::pair<bool,float> checkCollisionsWithLeft(const sf::FloatRect &collider);

    int load();

    void render(sf::RenderTarget&, sf::View&);
    void update();
    void merge();
    void hole(const sf::Vector2i &pos, const uint32_t &intensity);
    void hole_not_recursive(const sf::Vector2i &pos, const uint32_t &intensity);

    void showDebug();
    void hideDebug();

    std::vector <sf::Vector2f> explosion_points;

private:

    sf::Shader shader;

    std::thread explode;
    std::vector <sf::Sprite> rects;

    Voxel grid [gw][gl];

    bool debug = false;
    int indexX = 0;
    int y = 0;
    int x = 0;

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