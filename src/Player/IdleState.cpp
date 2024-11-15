#include "IdleState.hpp"

void IdleState::enter() {

}

void IdleState::update(IntPhysicsComponent& physComp, float dt) {
    physComp.velocity.x -= physComp.velocity.x / 10;
}

void IdleState::draw(sf::RenderTarget &window, sf::Sprite &sprite){
    sprite.setColor(sf::Color::White);
    sprite.setTextureRect(sf::IntRect(0,0, PlayerGlobal::characterSize.x, PlayerGlobal::characterSize.y));
    window.draw(sprite);
}

void IdleState::input(bool grounded) {
    if(Controls::moveLeft() || Controls::moveRight()) {
        PlayerState::currentState = walkState;
    } if(Controls::jump()) {
        if(!grounded) return;
        PlayerState::currentState = jumpState;
        PlayerState::currentState->enter();
    }
}
