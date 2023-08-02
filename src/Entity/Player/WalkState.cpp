#include "WalkState.hpp"

void WalkState::enter() {

}

void WalkState::update(sf::Vector2f&pos,float&,float dt) {
    last_pos = pos;
    if(Controls::moveLeft()) {
        pos.x-=speed*dt;
        return;
    } else if(Controls::moveRight()) {
        pos.x+=speed*dt;
        return;
    }
    PlayerState::currentState = PlayerState::idleState;
}

void WalkState::draw(sf::RenderTarget &window, sf::Sprite &sprite){
    int offset_x = ((int)timer.getElapsedTime().asMilliseconds()/100)*18;
    sprite.setTextureRect(sf::IntRect(
        offset_x
        ,(last_pos.x < sprite.getPosition().x)*26,18,26));
    if(offset_x >= 5*18) {
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
