#include "Frog.hpp"

void Frog::load() {
    texture.loadFromFile("res/img/Mob/frog.png");
    sprite.setTexture(texture);

    default_behaviour.default_load();

    maxHealth = 10;
    health = maxHealth;

    mobType = MobType::Fish;
    currentState = FrogStateType::FrogIdle;
}

void Frog::update(const float dt) {

    if(scaredTimer > 0) {
        scaredTimer--;
        if(mobInvoke.playerSubVector.x < 0.f) {
            physicsComponent.velocity.x = frogSpeed;
        } else if(mobInvoke.playerSubVector.x > 0.f) {
            physicsComponent.velocity.x = -frogSpeed;
        }

        if(currentState != FrogStateType::FrogJump) {
            currentState = FrogStateType::FrogJump;
            states.at(currentState)->enter();
        }
    }

    sprite.setTexture(texture);

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
        FrogStateType::Type frogStateType = states.at(currentState)->changeTo();
        if(frogStateType != FrogStateType::No) {
            currentState = frogStateType;
            states.at(currentState)->enter();
        }
    }

    if(grounded)
        physicsComponent.velocity.x -= physicsComponent.velocity.x / 10.f;
}

void Frog::collisionCheck(VoxelManager &voxelManager) {

    default_behaviour.update(physicsComponent.transform_position, "Frog", health, voxelManager);
    remove_mob = default_behaviour.remove_mob;

    CollisionManager::CollisionState res = CollisionManager::handleCollisionsWith(physicsComponent, voxelManager.getChunkIndexer(), sf::Vector2u(16, 16));

    if(res.hasCollision) {
        physicsComponent.velocity.y = 0;
    }

    if(res.isLiquidContact) {
        // TODO : SWIM
    } else {
        // TODO : DONT SWIM
    }

    grounded = res.hasCollision;
}

void Frog::render(sf::RenderTarget &target) {
    states.at(currentState)->draw(target, sprite);

    default_behaviour.default_render(target);
}

void Frog::invoke(const MobInvoke &inv) {
    mobInvoke = inv;

    if(mobInvoke.distanceToPlayer <= distanceWhenInvoked) {
        scaredTimer = scaredTime;
    }

    if(inv.damage > 0) {
        default_behaviour.generalDamageBehaviour(health, inv.damage);

        currentState = FrogStateType::FrogDamage;
        states.at(currentState)->enter(); 
    }
} 

bool Frog::remove(VoxelManager& vx) {
    if(remove_mob) {
        sf::Vector2i pos = sf::Vector2i(physicsComponent.transform_position);

        vx.getChunkIndexer().boundVector(pos);

        default_behaviour.default_death(pos.x, pos.y, vx);
    }

    return remove_mob;
}

PhysicsComponent &Frog::getPhysicsComponent() {
    return physicsComponent;
}
