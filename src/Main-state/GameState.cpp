#include "GameState.hpp"

void GameState::update() {
    delta_T = deltaClock.getElapsedTime().asMilliseconds();
    if(slowmo) delta_T = 0.1f;
    deltaClock.restart();

    bg_shader.setUniform("time",shader_time.getElapsedTime().asSeconds());
    bg_shader.setUniform("resolution", sf::Vector2f(1,1));

    
    auto res = vx_manager.checkCollisionsWith(player.getBottomHitbox()); // Ground
    auto res2 = vx_manager.checkCollisionsWithInv(player.getTopHitbox());   // Top
    auto res3 = vx_manager.checkCollisionsWithLeft(player.getLeftHitbox());     // Left
    auto res4 = vx_manager.checkCollisionsWithRight(player.getRightHitbox());   // Right

    player.setGrounded(false);

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

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::H)) 
        vx_manager.hideDebug();
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Y)) 
        vx_manager.showDebug();
    if(ev.type == sf::Event::KeyReleased) {
        if(ev.key.code == sf::Keyboard::Z) {
            vx_manager.hole(sf::Vector2i((int)player.get_voxel_pos().x, (int)player.get_voxel_pos().y),1005);
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
    gun.update(vx_manager, sf::Vector2f(window.mapPixelToCoords(sf::Mouse::getPosition())), player.get_voxel_pos(), delta_T);
    window.clear();

    view.setCenter(player.get_voxel_pos().x, player.get_voxel_pos().y);
    window.setView(view);

    window.draw(background, &bg_shader);

    player.update(delta_T);
    player.draw(window);

    gun.render(window);

    vx_manager.render(window, view);
}
