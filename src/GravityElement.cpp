#include "GravityElement.hpp"

void GravityElement::update(ChunkIndexer &world) {
    if(first) {
        setVoxelInWorld(world); first = false;
    }

    sf::Vector2i lastPos = *this;
    sf::Vector2i nextWaterPos = *this;

    checkExisting(world);
    move_(nextWaterPos);

    /*sf::Vector2i pos = world.pointLineContainMaterial(lastPos, nextWaterPos);

    if(pos != sf::Vector2i(0, 0)) {
        velocity.y = 0;
        nextWaterPos = pos;
    }*/


    if(world.boundGetVoxelAt(nextWaterPos.x, nextWaterPos.y).value != 0) {
        // If position inside another voxel
        nextWaterPos.y -= velocity.y;
        velocity.y = 0;

        // If new value
        if(world.boundGetVoxelAt(nextWaterPos.x, nextWaterPos.y).value == 0) {
            clearLastPos(nextWaterPos, world);
        }
    } else {
        clearLastPos(nextWaterPos, world);

        run_rules(world, nextWaterPos);
    }

    setVoxelInWorld(world);
    setPosition(nextWaterPos);
    
    // Set 'needs_update' flags
    if(nextWaterPos != lastPos) {
        sf::Vector2i boundPos = nextWaterPos;
        sf::Vector2i chunk_pos = world.getChunkFromPos(boundPos.x, boundPos.y);

        world.boundGetChunkAt(chunk_pos.x, chunk_pos.y).needs_update = true;
        world.boundGetChunkAt(chunk_pos.x + 1, chunk_pos.y + 1).needs_update = true;
        world.boundGetChunkAt(chunk_pos.x + 1, chunk_pos.y - 1).needs_update = true;

        world.boundGetChunkAt(chunk_pos.x - 1, chunk_pos.y + 1).needs_update = true;
        world.boundGetChunkAt(chunk_pos.x - 1, chunk_pos.y - 1).needs_update = true;

        world.boundGetChunkAt(chunk_pos.x - 1, chunk_pos.y).needs_update = true;
        world.boundGetChunkAt(chunk_pos.x + 1, chunk_pos.y).needs_update = true;

        world.boundGetChunkAt(chunk_pos.x, chunk_pos.y + 1).needs_update = true;
        world.boundGetChunkAt(chunk_pos.x, chunk_pos.y - 1).needs_update = true;
    }
    else 
        world.boundGetChunkAt(world.getChunkFromPos(lastPos.x, lastPos.y).x, world.getChunkFromPos(lastPos.x, lastPos.y).y).needs_update = false;

    // Draw voxel
    world.boundGetVoxelAt(x, y).value = value;
    world.boundSetImagePixelAt(x, y, color);
}

void GravityElement::move_(sf::Vector2i &nextWaterPos) {
    velocity.y += 1;
    nextWaterPos += velocity;
}

void GravityElement::setVoxelInWorld(ChunkIndexer &world)
{
    world.boundSetImagePixelAt(x, y, color);
    world.boundGetVoxelAt(x, y).value = value;
}

void GravityElement::clearLastPos(const sf::Vector2i &nextWaterPos, ChunkIndexer &world)
{
    world.boundSetImagePixelAt(x, y, sf::Color(0,0,0,0));
    
    world.boundGetVoxelAt(x, y).value = 0;
}

void GravityElement::setPosition(sf::Vector2i& nextWaterPos) {
    x = nextWaterPos.x;
    y = nextWaterPos.y;
}


void GravityElement::checkExisting(ChunkIndexer &world) {
    if(world.boundGetVoxelAt(x, y).value == 0) {
        remove = true;
        return;
    }
}
