#include "GameState.hpp"
#include "common.hpp"

void GameState::update() {
    delta_T = deltaClock.getElapsedTime().asMilliseconds();
    if(slowmo) delta_T = 0.2f;
    deltaClock.restart();

    shader.setUniform("time",shader_time.getElapsedTime().asSeconds());
    shader.setUniform("resolution",sf::Vector2(1.0f,1.0f));

    if(Controls::switchItem()) {
        inv.switchItem();
    }


    if(world.main_world.explosion_points.size() > 0) {
        effOverlay.effect_explosion(world.main_world.explosion_points.at(world.main_world.explosion_points.size() - 1));
        world.main_world.explosion_points.pop_back();
    }
    for(auto &wrl: world.add_worlds) {
        if(wrl.explosion_points.size() > 0) {
            effOverlay.effect_explosion(wrl.explosion_points.at(wrl.explosion_points.size() - 1));
            wrl.explosion_points.pop_back();
        }
    }

    bg.update(game_camera.getCenterPosition());
    BGMusic::update();
    effOverlay.update(game_camera.getCenterPosition());
    game_camera.update(delta_T);
    player.update(delta_T);    
    matUI.update(world.main_world);

    world.update();
        
    inv.getCurrentItem()->update(world, sf::Vector2f(renderTexture.mapPixelToCoords(sf::Vector2i(sf::Mouse::getPosition()))), player.get_voxel_pos(), delta_T);

    world.handleCollisionsWithPlayer(player);
}

void GameState::input(sf::Event &ev) {

    // Toggle slow motion (set delta time to 0.1)
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::F)) slowmo = true; // Slow-mo
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)) slowmo = false; // Slow-mo

    if(Controls::useItem(ev)) {
        inv.use(player.get_voxel_pos(), sf::Vector2f(renderTexture.mapPixelToCoords(sf::Mouse::getPosition())), world);    
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
    world.save();
}

void GameState::draw(sf::RenderTarget &window)
{
    // Clear renderTexture
    renderTexture.clear();

    game_camera.setViewTo(renderTexture);
    game_camera.setTarget(player.get_voxel_pos());

    // Render background
    bg.render(renderTexture);

    // Render player and gun
    player.draw(renderTexture);

    // Render the world
    world.render(renderTexture, game_camera.getCenterPosition());

    // For effects like explosions
    effOverlay.render(renderTexture);
    // Render current item
    inv.render(renderTexture);


    ui_camera.setViewTo(renderTexture);
    matUI.render(renderTexture);

    game_camera.setViewTo(renderTexture);
    
    // Display, draw and set shader
    renderTexture.display();
    renderSprite.setTexture(renderTexture.getTexture());
    window.draw(renderSprite , &shader);

    float currentTime = clock.restart().asMilliseconds();
    float fps = 1.f / (currentTime - lastTime);
    lastTime = currentTime;
}
