#include "WalkState.hpp"

void WalkState::enter() {

}
 
void WalkState::update(IntPhysicsComponent& physComp,float dt) {
    last_pos = sf::Vector2i(physComp.transform_position);

    physComp.velocity.x = 0;

    if(Controls::moveLeft()) 
        physComp.velocity.x = -speed;
    if(Controls::moveRight())
        physComp.velocity.x = speed;

    PlayerState::currentState = PlayerState::idleState;
}

void WalkState::draw(sf::RenderTarget &window, sf::Sprite &sprite){

    const int frames = PlayerGlobal::character_animation_frames;
    const sf::Vector2u frameSize = PlayerGlobal::characterSize;

    const bool isMoving = last_pos.x < sprite.getPosition().x;

    const int time = timer.getElapsedTime().asMilliseconds() / 80;

    const sf::Vector2u offset = sf::Vector2u(time * frameSize.x,  isMoving * frameSize.y);

    const sf::IntRect textureRect = sf::IntRect(sf::Vector2i(offset), sf::Vector2i(frameSize));

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
