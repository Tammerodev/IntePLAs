#include "JumpState.hpp"

void JumpState::enter() {
}

void JumpState::update(sf::Vector2f&pos,float&yacc,float dt) {
    yacc = -5;
    PlayerState::currentState = idleState;
}

void JumpState::draw(sf::RenderTarget &window, sf::Sprite &sprite){
    sprite.setTextureRect(sf::IntRect(0,0,18,26));
    window.draw(sprite);
}

void JumpState::input(bool grounded) {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        PlayerState::currentState = walkState;
    } if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        if(grounded)
        PlayerState::currentState = jumpState;
    }
}
