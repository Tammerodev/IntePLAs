#include "JumpState.hpp"

void JumpState::enter() {
}

void JumpState::update(IntPhysicsComponent& physComp, float dt) {
    physComp.velocity_buffer = -6.0;

    PlayerState::currentState = idleState;
}

void JumpState::draw(sf::RenderTarget &window, sf::Sprite &sprite){
    sprite.setTextureRect(sf::IntRect(0,0,18,26));
    window.draw(sprite);
}

void JumpState::input(bool grounded) {
    if(Controls::moveLeft() || Controls::moveRight()) {
        PlayerState::currentState = walkState;
    } if(Controls::jump()) {
        if(grounded)
        PlayerState::currentState = jumpState;
    }
}
