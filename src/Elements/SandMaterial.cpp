#include "SandMaterial.hpp"

void SandMaterial::move_(sf::Vector2i &nextWaterPos) {
    velocity.y += 1;
    nextWaterPos += velocity;
}