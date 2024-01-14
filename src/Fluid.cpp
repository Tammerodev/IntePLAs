#include "Fluid.hpp"

void Fluid::update(ChunkIndexer &world) {
    sf::Vector2i lastPos = *this;
    sf::Vector2i nextWaterPos = *this;

    
    setVoxelInWorld(world);
    checkExisting(world);
    move(nextWaterPos);

    if(world.boundGetVoxelAt(nextWaterPos.x, nextWaterPos.y).value == 0) {
        clearLastPos(nextWaterPos, world);
    } else {
        // If position inside another voxel
        nextWaterPos.y -= velocity.y;
        velocity.y = 0;

        sf::Vector2i pos = world.pointLineContainMaterial(lastPos, nextWaterPos);
        if(pos != sf::Vector2i(0, 0)) 
            nextWaterPos = pos;

        // res means x direction
        int res = 0;

        if(world.boundGetVoxelAt(nextWaterPos.x + 1, nextWaterPos.y).value == 0) {
            res = 1;
        }
        if(world.boundGetVoxelAt(nextWaterPos.x - 1, nextWaterPos.y).value == 0) {
            res = -1;
        }
        if(world.boundGetVoxelAt(nextWaterPos.x - 1, nextWaterPos.y).value == 0 && world.boundGetVoxelAt(nextWaterPos.x + 1, nextWaterPos.y).value == 0) {
            res = math::randIntInRange(-1, 1);
        }

        // Move by 'res'
        nextWaterPos.x += res;

        // If new value
        if(world.boundGetVoxelAt(nextWaterPos.x, nextWaterPos.y).value == 0) {
            clearLastPos(nextWaterPos, world);
        }
    }
    
    // Set 'needs_update' flags
    if(nextWaterPos != lastPos) {
        sf::Vector2i boundPos = lastPos;
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

void Fluid::move(sf::Vector2i &nextWaterPos) {
    velocity.y += 1;
    nextWaterPos += velocity;
}

void Fluid::setVoxelInWorld(ChunkIndexer &world)
{
    world.boundSetImagePixelAt(x, y, color);
    world.boundGetVoxelAt(x, y).value = value;
}

void Fluid::clearLastPos(const sf::Vector2i &nextWaterPos, ChunkIndexer &world)
{
    world.boundSetImagePixelAt(x, y, sf::Color(0,0,0,0));
    
    world.boundGetVoxelAt(x, y).value = 0;

    x = nextWaterPos.x;
    y = nextWaterPos.y;
}

void Fluid::checkExisting(ChunkIndexer &world) {
    if(world.boundGetVoxelAt(x, y).value == 0) {
        remove = true;
        return;
    }
}
