#include "GravityElement.hpp"

void GravityElement::update(ChunkIndexer &world) {
    if(first) {
        first = false;
    }

    world.boundVector(*this);

    sf::Vector2i lastPos = *this;
    sf::Vector2i nextPos = *this;

    //if(checkExisting(world)) 
    //    return;                         // Return if voxel does not exist (destroyed)

    move_(nextPos);                     // Calculate velocity and position
    custom_update(world, nextPos);      // Call virtual update method

    if(abs(velocity.y) > terminal_velocity) // Bound velocity
        velocity.y--;                   

    sf::Vector2i pos = world.pointLineContainMaterial(lastPos, nextPos);

    if(pos != sf::Vector2i(0, 0)) {         // Function returns {0,0} if no material found in line
        velocity.y = 0;
        nextPos = pos;                      // If material found, the function returns the point before material found
    }

    if(world.boundGetVoxelAt(nextPos.x, nextPos.y).value != 0) {    // Check if we are inside another voxel
        nextPos.y -= velocity.y;

        if(world.boundGetVoxelAt(nextPos.x, nextPos.y).value == 0) {
            clearLastPos(world);
        }
    }

    world.boundVector(nextPos);

    clearLastPos(world);
    setPosition(nextPos);
    run_rules(world, nextPos);
    setPosition(nextPos);

    world.boundVector(nextPos);
    const sf::Vector2i& chunk_pos = world.getChunkFromPos(nextPos.x, nextPos.y);


    if (nextPos == lastPos) {   // If we havent moved, updating is unnecessary
        world.boundGetChunkAt(chunk_pos.x, chunk_pos.y).needs_update = false;
    } else {
        // If we have moved
        // Set 3x3 chunks to update
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                world.boundGetChunkAt(chunk_pos.x + dx, chunk_pos.y + dy).needs_update = true;
            }
        }
    }

    world.boundVector(*this);
    setVoxelInWorld(world);
    lastPos = *this;
}

void GravityElement::move_(sf::Vector2i &nextWaterPos) {
    velocity.y += 1;
    nextWaterPos += velocity;
}

void GravityElement::setVoxelInWorld(ChunkIndexer &world)
{
    world.boundVector(*this);

    world.setImagePixelAt(x, y, color);
    Voxel& vox = world.getVoxelAt(x, y);
    vox.value = value;

    int avarage = (vox.temp + temperature) / 2;
    vox.temp = avarage;
}

void GravityElement::clearLastPos(ChunkIndexer &world)
{
    world.boundClearVoxelAt(x, y);
}

void GravityElement::setPosition(sf::Vector2i& nextWaterPos) {
    x = nextWaterPos.x;
    y = nextWaterPos.y;
}


bool GravityElement::checkExisting(ChunkIndexer &world) {
    sf::Vector2i v = world.getBoundedVector(sf::Vector2i(x, y));

    if(world.getVoxelAt(v.x, v.y).value == 0) {
        world.clearVoxelAt(v.x, v.y);
        remove = true;
    }

    return remove;
}