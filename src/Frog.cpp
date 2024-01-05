#include "Frog.hpp"

FrogState* FrogState::damageState = new FrogDamageState();
FrogState* FrogState::idleState = new FrogIdleState();
FrogState* FrogState::dead = new FrogDeadState();
FrogState* FrogState::jumpState = new FrogJumpState();

FrogState* FrogState::currentState = FrogState::idleState;

void Frog::load() {
    texture.loadFromFile("res/img/Mob/frog.png");
    sprite.setTexture(texture);

    physicsComponent.transform_position = sf::Vector2f(800, 0);
}

 void Frog::update(const float dt) {

    if(scaredTimer > 0) {
        scaredTimer--;
        if(mobInvoke.playerSubVector.x < 0.f) {
            physicsComponent.velocity.x = frogSpeed;
        } else if(mobInvoke.playerSubVector.x > 0.f) {
            physicsComponent.velocity.x = -frogSpeed;
        }

        if(FrogState::currentState != FrogState::jumpState) {
            FrogState::currentState = FrogState::jumpState;
            FrogState::currentState->enter();
        }
    }

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

    if(grounded)
        physicsComponent.velocity.x -= physicsComponent.velocity.x / 10.f;
}

void Frog::collisionCheck(VoxelManager &voxelManager) {

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
    FrogState::currentState->draw(target, sprite);
}

void Frog::invoke(const MobInvoke &inv) {
    mobInvoke = inv;

    if(mobInvoke.distanceToPlayer <= distanceWhenInvoked) {
        scaredTimer = scaredTime;
    }
}

bool Frog::remove() {
    return false;
}

PhysicsComponent &Frog::getPhysicsComponent() {
    return physicsComponent;
}
