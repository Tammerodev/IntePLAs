#include "RespawnState.hpp"

void RespawnState::enter() {

}

void RespawnState::update(IntPhysicsComponent& physComp, float dt) {
    if(firstTime) {
        physComp.transform_position = sf::Vector2f(1000, -300);
    }

    physComp.velocity_buffer /= 1.001;

    firstTime = false;
}

void RespawnState::draw(sf::RenderTarget &window, sf::Sprite &sprite) {
    sprite.setTextureRect(sf::IntRect(0,0,18,28));
    sprite.setColor(sf::Color::Blue);
    window.draw(sprite);
}

void RespawnState::input(bool grounded) {
    if(grounded) {
        PlayerState::currentState = PlayerState::idleState;
        PlayerState::currentState->enter();
    }
}
