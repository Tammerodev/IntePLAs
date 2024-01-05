#include "Fish.hpp"

void Fish::load() {
    texture.loadFromFile("res/img/Mob/fish.png");
    sprite.setTexture(texture);

    const int seed = time(0);
    time_offset = rand() % 10000;

    fsl.SetNoiseType(FastNoiseLite::NoiseType::NoiseType_Perlin);
    fsl.SetFractalOctaves(13);
    fsl.SetFractalGain(10.0);
    fsl.SetFractalLacunarity(50.5);
    
    fsl.SetSeed(seed); // Set a random seed (change this to get different noise patterns)
}

void Fish::update(const float dt) {

    sprite.setTexture(texture);

    FrogState::currentState->update(physicsComponent, dt, grounded);

    physicsComponent.update(dt);
    sprite.setPosition(physicsComponent.transform_position);

    if(physicsComponent.velocity.x > 0.f) {
        sprite.setOrigin({ sprite.getLocalBounds().width, 0 });
        sprite.setScale(-1.0, 1.0f);
    } else {
        sprite.setOrigin({ 0, 0 });
        sprite.setScale(1.0, 1.0f);
    }

}

void Fish::collisionCheck(VoxelManager &voxelManager) {

    CollisionManager::CollisionState res = CollisionManager::handleCollisionsWith(physicsComponent, voxelManager.getChunkIndexer(), sf::Vector2u(16, 16));

    if(res.hasCollision) {
        physicsComponent.velocity.y = 0;
    }

    if(res.isLiquidContact) {
        // TODO : SWIM
        sf::Vector2f velocity_add;
        velocity_add.x = fsl.GetNoise((timer.getElapsedTime().asMilliseconds() + time_offset) / 30.f, 0.f);
        velocity_add.y = fsl.GetNoise(0.f, (timer.getElapsedTime().asMilliseconds() + time_offset) / 30.f);

        physicsComponent.velocity = velocity_add / speed_division;
    } else {
        //do_remove = true;
        physicsComponent.velocity.x /= 2.f;
    }

    grounded = res.hasCollision;
}

void Fish::render(sf::RenderTarget &target) {
    FrogState::currentState->draw(target, sprite);
}

void Fish::invoke(const MobInvoke &inv) {
    mobInvoke = inv;

    if(mobInvoke.distanceToPlayer <= distanceWhenInvoked) {
        scaredTimer = scaredTime;
    }
}

bool Fish::remove() {
    return do_remove;
}

PhysicsComponent &Fish::getPhysicsComponent() {
    return physicsComponent;
}
