#include "CreativeState.hpp"

void CreativeState::enter() {

}
 
void CreativeState::update(IntPhysicsComponent& physComp,float dt) {
    physComp.velocity_buffer = 0;
    
    last_pos = sf::Vector2i(physComp.transform_position);

    physComp.velocity.x = 0;
    physComp.velocity.y = 0;


    if(Controls::moveLeft()) 
        physComp.velocity.x = -speed;
    if(Controls::moveRight())
        physComp.velocity.x = speed;
    if(Controls::jump()) 
        physComp.velocity.y = -speed;
    if(Controls::moveDown())
        physComp.velocity.y = speed;
}

void CreativeState::draw(sf::RenderTarget &window, sf::Sprite &sprite){

    const int frames = 5;
    const sf::Vector2i frameSize = sf::Vector2i(18, 28);

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

void CreativeState::input(bool grounded) {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K)) speed++;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I)) speed--;

}
