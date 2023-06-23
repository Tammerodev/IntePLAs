#include "IdleState.hpp"

void IdleState::enter() {

}

void IdleState::update(sf::Vector2f&,float&, float dt) {

}

void IdleState::draw(sf::RenderTarget &window, sf::Sprite &sprite){
    sprite.setTextureRect(sf::IntRect(0,0,18,26));
    window.draw(sprite);
}

void IdleState::input(bool grounded) {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        PlayerState::currentState = walkState;
    } if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        if(!grounded) return;
        PlayerState::currentState = jumpState;
        PlayerState::currentState->enter();
    }
}
