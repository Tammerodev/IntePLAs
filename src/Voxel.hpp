#pragma once
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include "Elements.hpp"


class Voxel {
    public:
    
    // Bitfield (4 bits, 16 combinations)
    unsigned char value : 4 = 0;

    unsigned char strenght = 1;
    bool used = false;
    bool isFalling = false;
    // Bitfield (11 bits, 1023 combinations)
    unsigned short temp = 0;
    
    private:
};


const Voxel getValueFromCol(const sf::Color &px, sf::Vector2i p);