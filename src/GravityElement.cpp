#include "GravityElement.hpp"

void GravityElement::update(ChunkIndexer &world) {
    if(first) {
        first = false;
    }

    sf::Vector2i lastPos = *this;
    sf::Vector2i nextWaterPos = *this;

    if(checkExisting(world)) {
        return;
    }

    move_(nextWaterPos);
    custom_update(world, nextWaterPos);

    if(abs(velocity.y) > terminal_velocity) {
        velocity.y--;
    }

    sf::Vector2i pos = world.pointLineContainMaterial(lastPos, nextWaterPos);

    if(pos != sf::Vector2i(0, 0)) {
        velocity.y = 0;
        nextWaterPos = pos;
    }

    auto nextVoxelValue = world.boundGetVoxelAt(nextWaterPos.x, nextWaterPos.y).value;
    if(nextVoxelValue != 0) {
        // If position inside another voxel
        nextWaterPos.y -= velocity.y;

        // If new value
        if(nextVoxelValue == 0) {
            world.boundVector(nextWaterPos);
            clearLastPos(world);
        }
    }

    //
    world.boundVector(nextWaterPos);

    clearLastPos(world);
    setPosition(nextWaterPos); 
    run_rules(world , nextWaterPos);
    setPosition(nextWaterPos); 

    // Set 'needs_update' flags
    const sf::Vector2i& boundPos = nextWaterPos;
    const sf::Vector2i& chunk_pos = world.getChunkFromPos(boundPos.x, boundPos.y);

    if (nextWaterPos == lastPos) {
        world.boundGetChunkAt(chunk_pos.x, chunk_pos.y).needs_update = false;
    } else {
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                world.boundGetChunkAt(chunk_pos.x + dx, chunk_pos.y + dy).needs_update = true;
                prndd("p");
            }
        }
    }

    setVoxelInWorld(world);

    lastPos = *this;
}

void GravityElement::move_(sf::Vector2i &nextWaterPos) {
    velocity.y += 1;
    nextWaterPos += velocity;
}

void GravityElement::setVoxelInWorld(ChunkIndexer &world)
{
    world.boundSetImagePixelAt(x, y, color);
    Voxel& vox =  world.boundGetVoxelAt(x, y);
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
