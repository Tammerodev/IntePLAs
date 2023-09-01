#include "WalkState.hpp"

void WalkState::enter() {

}
 
void WalkState::update(PhysicsComponent& physComp,float dt) {
    last_pos = physComp.transform_position;

    physComp.velocity.x = std::clamp(physComp.velocity.x, -speed, speed);

    if(Controls::moveLeft()) {
        physComp.velocity.x -= acc;
        return;
    } 
    if(Controls::moveRight()) {
        physComp.velocity.x += acc;
        return;
    }


    PlayerState::currentState = PlayerState::idleState;
}

void WalkState::draw(sf::RenderTarget &window, sf::Sprite &sprite){

    const int frames = 5;
    const sf::Vector2i frameSize = sf::Vector2i(18, 26);

    const bool isMoving = last_pos.x < sprite.getPosition().x;

    const int time = timer.getElapsedTime().asMilliseconds() / 100;

    const sf::Vector2i offset = sf::Vector2i(time * frameSize.x,  isMoving * frameSize.y);

    const sf::IntRect textureRect = sf::IntRect(offset, frameSize);

    sprite.setTextureRect(sf::IntRect(textureRect));

    if(offset.x >= frames*frameSize.x) {
        timer.restart();
    }

    window.draw(sprite);
}

void WalkState::input(bool grounded) {
    if(Controls::jump() && grounded) {
        PlayerState::currentState = jumpState;
        PlayerState::currentState->enter();
    }
}
