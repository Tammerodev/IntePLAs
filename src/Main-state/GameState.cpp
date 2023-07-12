#include "GameState.hpp"
#include "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/src/common.hpp"

void GameState::update() {
    delta_T = deltaClock.getElapsedTime().asMilliseconds();
    if(slowmo) delta_T = 0.1f;
    deltaClock.restart();

    shader.setUniform("time",shader_time.getElapsedTime().asSeconds());
    shader.setUniform("resolution",sf::Vector2(1.0f,1.0f));


    if(vx_manager.explosion_points.size() > 0) {
        effOverlay.effect_explosion(vx_manager.explosion_points.at(vx_manager.explosion_points.size() - 1));
        vx_manager.explosion_points.pop_back();
    }

    bg.update();
    BGMusic::update();
    effOverlay.update(game_camera.getCenterPosition());
    game_camera.update(delta_T);
    player.update(delta_T);    
    gun.update(vx_manager, sf::Vector2f(renderTexture.mapPixelToCoords(sf::Mouse::getPosition())), player.get_voxel_pos(), delta_T);
    placer.update(vx_manager, sf::Vector2f(renderTexture.mapPixelToCoords(sf::Mouse::getPosition())), player.get_voxel_pos(), delta_T);
    vx_manager.update();

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

    // Toggle slow motion (set delta time to 0.1)
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::F)) slowmo = true; // Slow-mo
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)) slowmo = false; // Slow-mo

    if(ev.type == sf::Event::MouseButtonPressed) {
        if(ev.mouseButton.button == sf::Mouse::Button::Left) {
            if(inv::currTool != inv::RocketLauncher) return;
            SFX::rocket_launcher_fire.play();
            gun.spawn_bullet(player.get_voxel_pos());    
        }
        if(ev.mouseButton.button == sf::Mouse::Button::Right) {
            if(inv::currTool == inv::DebugPlacer)
            placer.use(sf::Vector2i(renderTexture.mapPixelToCoords(sf::Mouse::getPosition())));
        }

    }
    if(ev.type == sf::Event::KeyReleased) {
        if(ev.key.code == sf::Keyboard::Q) {
        switch (inv::currTool) {
        case inv::DebugPlacer:
            inv::currTool = inv::RocketLauncher;
            break;
        case inv::RocketLauncher:
            inv::currTool = inv::DebugPlacer;
            break;
        }  

        }
        if(ev.key.code == sf::Keyboard::Escape) {
            GameState::currentState = GameState::menuState;
            menuState->load();
        }
    }
}

void GameState::draw(sf::RenderTarget &window)
{
    // Clear renderTexture
    renderTexture.clear();

    // Update camera
    game_camera.setViewTo(renderTexture);
    game_camera.setTarget(player.get_voxel_pos());

    // Render background
    bg.render(renderTexture);

    // Render player and gun
    player.draw(renderTexture);

    switch (inv::currTool)
    {
    case inv::DebugPlacer:
        placer.render(renderTexture);
        break;
    case inv::RocketLauncher:
        gun.render(renderTexture);
        break;
    }   

    // Render the world
    vx_manager.render(renderTexture);

    // For effects like explosions
    effOverlay.render(renderTexture);
    
    // Display, draw and set shader
    renderTexture.display();
    renderSprite.setTexture(renderTexture.getTexture());
    window.draw(renderSprite , &shader);
}
