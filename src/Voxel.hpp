#pragma once
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include "Elements.hpp"


class Voxel {
    public:
    
    unsigned char value = 0;
    unsigned char strenght = 1;
    bool used = false;
    bool isFalling = false;
    short temp = .0f;
    
    private:
};


const Voxel getValueFromCol(const sf::Color &px, sf::Vector2i p);