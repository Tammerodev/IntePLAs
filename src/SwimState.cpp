#include "SwimState.hpp"

void SwimState::enter() {

}

void SwimState::update(IntPhysicsComponent &physComp, float) {
    last_pos = sf::Vector2i(physComp.transform_position);
    physComp.velocity.x -= physComp.velocity.x / water_slowdown_x;

    if(Controls::moveLeft()) 
        physComp.velocity.x = -speed;
    if(Controls::moveRight())
        physComp.velocity.x = speed;

    physComp.velocity_buffer -= physComp.velocity_buffer / water_slowdown_y;
}

void SwimState::draw(sf::RenderTarget &window, sf::Sprite &sprite) {

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

void SwimState::input(bool grounded) {
    if(Controls::jump() && swimCoolDown.getElapsedTime().asSeconds() > 0.75) {
        swimCoolDown.restart();
        PlayerState::currentState = jumpState;
        PlayerState::currentState->enter();
    }
}
