#include "GameState.hpp"
#include "common.hpp"

void GameState::update() {
    delta_T = deltaClock.getElapsedTime().asMilliseconds();
    if(slowmo) delta_T = 0.2f;
    deltaClock.restart();

    shader.setUniform("time",shader_time.getElapsedTime().asSeconds());
    shader.setUniform("resolution",sf::Vector2(1.0f,1.0f));


    if(vx_manager.explosion_points.size() > 0) {
        effOverlay.effect_explosion(vx_manager.explosion_points.at(vx_manager.explosion_points.size() - 1));
        vx_manager.explosion_points.pop_back();
    }

    bg.update(game_camera.getCenterPosition());
    BGMusic::update();
    effOverlay.update(game_camera.getCenterPosition());
    game_camera.update(delta_T);
    player.update(delta_T);    

    vx_manager.update();
        
    inv.getCurrentItem()->update(vx_manager, sf::Vector2f(renderTexture.mapPixelToCoords(sf::Vector2i(sf::Mouse::getPosition()))), player.get_voxel_pos(), delta_T);

    auto res = vx_manager.getOvelapWithRectY(player.getBottomHitbox()); // Ground
    auto res2 = vx_manager.getOvelapWithRect(player.getTopHitbox()); // Ground
    auto res4 = vx_manager.getOvelapWithRect(player.getRightHitbox()); // Ground
    auto res3 = vx_manager.getOvelapWithRect(player.getLeftHitbox()); // Ground

    if(res4.first) {    // Right collision
        player.move_x(-(res4.second.width + 2));
    }
    if(res3.first) {    // Right collision
        player.move_x((res3.second.width + 2));
    }
    if(res.first) {     // Colliding with ground
        player.ground();
        player.move_y(-res.second.height);
        return;
    }

    if(res2.first) {    // Head collision
        player.move_y(res2.second.height);
    }
}

void GameState::input(sf::Event &ev) {

    // Toggle slow motion (set delta time to 0.1)
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::F)) slowmo = true; // Slow-mo
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)) slowmo = false; // Slow-mo

    if(ev.type == sf::Event::MouseButtonPressed) {
        if(ev.mouseButton.button == sf::Mouse::Button::Left) {
            inv.use(player.get_voxel_pos(), sf::Vector2f(renderTexture.mapPixelToCoords(sf::Mouse::getPosition())));    
        } else if(ev.mouseButton.button == sf::Mouse::Button::Middle) {
            inv.switchItem();
        }
    }
    if(ev.type == sf::Event::KeyReleased) {
        if(ev.key.code == sf::Keyboard::Escape) {
            statexit();
            GameState::currentState = GameState::menuState;
            menuState->load("");
        }
    }
}

void GameState::statexit()
{
    BGMusic::stop();
    vx_manager.save();
}

void GameState::draw(sf::RenderTarget &window)
{
    // Clear renderTexture
    renderTexture.clear();
    // Render to game camera
    game_camera.setViewTo(renderTexture);
    game_camera.setTarget(player.get_voxel_pos());

    // Render background
    bg.render(renderTexture);

    // Render player and gun
    player.draw(renderTexture);

    // Render the world
    vx_manager.render(renderTexture, game_camera.getCenterPosition());

    // For effects like explosions
    effOverlay.render(renderTexture);
    // Render current item
    inv.render(renderTexture);
    
    // Display, draw and set shader
    renderTexture.display();
    renderSprite.setTexture(renderTexture.getTexture());
    window.draw(renderSprite , &shader);

    float currentTime = clock.restart().asMilliseconds();
    float fps = 1.f / (currentTime - lastTime);
    lastTime = currentTime;
}
