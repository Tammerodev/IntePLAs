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

    maxHealth = 10;
    health = maxHealth;
    default_behaviour.default_load();

    mobType = MobType::Fish;
}

void Fish::update(const float dt) {

    sprite.setTexture(texture);
    
    if(states.at(currentState) != nullptr)
        states.at(currentState)->update(physicsComponent, dt, grounded);

    physicsComponent.update(dt);
    sprite.setPosition(physicsComponent.transform_position);

    if(physicsComponent.velocity.x > 0.f) {
        sprite.setOrigin({ sprite.getLocalBounds().width, 0 });
        sprite.setScale(-1.0, 1.0f);
    } else {
        sprite.setOrigin({ 0, 0 });
        sprite.setScale(1.0, 1.0f);
    }

    if(states.at(currentState) != nullptr) {
        FishStateType::Type fishStateType = states.at(currentState)->changeTo();
        if(fishStateType != FishStateType::No) {
            currentState = fishStateType;
            states.at(currentState)->enter();
        }
    }


}

void Fish::collisionCheck(VoxelManager &voxelManager) {
    default_behaviour.update(physicsComponent.transform_position, "Fish", health, voxelManager, MeatPropeties::fish);
    remove_mob = default_behaviour.remove_mob;

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
    if(states.at(currentState) != nullptr)
        states.at(currentState)->draw(target, sprite);

    default_behaviour.default_render(target);
}

void Fish::invoke(const MobInvoke &inv) {
    mobInvoke = inv;

    if(mobInvoke.distanceToPlayer <= distanceWhenInvoked) {
        scaredTimer = scaredTime;
    }

    if(inv.damage > 0) {
        default_behaviour.generalDamageBehaviour(health, inv.damage);

        currentState = FishStateType::FishDamage;
        if(states.at(currentState) != nullptr)
            states.at(currentState)->enter();
    }
}

bool Fish::remove(VoxelManager&) {
    return remove_mob;
}

PhysicsComponent &Fish::getPhysicsComponent() {
    return physicsComponent;
}
