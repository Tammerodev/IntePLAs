#include "GameState.hpp"

void GameState::update() {
    delta_T = deltaClock.getElapsedTime().asMilliseconds();
    if(slowmo) delta_T = 0.1f;
    deltaClock.restart();

    shader.setUniform("time",shader_time.getElapsedTime().asSeconds());

    bg.update();

    if(vx_manager.explosion_points.size() > 0) {
        effOverlay.effect_explosion(vx_manager.explosion_points.at(vx_manager.explosion_points.size() - 1));
        vx_manager.explosion_points.pop_back();
    }
    effOverlay.update(view.getCenter());

    player.update(delta_T);
    
    player.setGrounded(false);
    
    auto res = vx_manager.checkCollisionsWith(player.getBottomHitbox()); // Ground
    auto res2 = vx_manager.checkCollisionsWithInv(player.getTopHitbox());   // Top
    auto res3 = vx_manager.checkCollisionsWithLeft(player.getLeftHitbox());     // Left
    auto res4 = vx_manager.checkCollisionsWithRight(player.getRightHitbox());   // Right

    if(res3.first) { // Left collision
        player.move_x(res3.second);
        player.update_hitboxbottom();
    }
    else if(res4.first) {    // Right collision
        player.move_x(res4.second);
        player.update_hitboxbottom();
    }

    if(res.first) {     // Colliding with ground
        player.ground();
        player.setGrounded(true);
        player.move_y(res.second - 12.f);
        return;
    }

    if(res2.first) {    // Head collision
        player.ground();
        player.move_y(res2.second);
    }
}

void GameState::input(sf::Event &ev) {
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) slowmo = true; // Slow-mo
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) slowmo = false; // Slow-mo

    if(ev.type == sf::Event::KeyReleased) {
        if(ev.key.code == sf::Keyboard::Z) {
            vx_manager.hole(sf::Vector2i((int)player.get_voxel_pos().x, (int)player.get_voxel_pos().y),205);
        }
        else if(ev.key.code == sf::Keyboard::X) {
            vx_manager.hole(sf::Vector2i((int)player.get_voxel_pos().x, (int)player.get_voxel_pos().y),55u);
        }
        else if(ev.key.code == sf::Keyboard::E) {
            gun.spawn_bullet(player.get_voxel_pos());
        }
        else if(ev.key.code == sf::Keyboard::Escape) {
            GameState::currentState = GameState::menuState;
            menuState->load();
        }
    }
}

void GameState::draw(sf::RenderTarget &window)
{
    gun.update(vx_manager, sf::Vector2f(renderTexture.mapPixelToCoords(sf::Mouse::getPosition())), player.get_voxel_pos(), delta_T);
    renderTexture.clear();

    bg.render(renderTexture);
 

    renderTexture.setView(view);
    view.setCenter(player.get_voxel_pos().x, player.get_voxel_pos().y);
    player.draw(renderTexture);

    gun.render(renderTexture);

    vx_manager.render(renderTexture, view);

    effOverlay.render(renderTexture);

    renderTexture.display();

    renderSprite.setTexture(renderTexture.getTexture());
    window.draw(renderSprite , &shader);
}
