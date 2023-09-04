#include "GameState.hpp"
#include "common.hpp"

bool GameState::load(const std::string s, tgui::BackendGui& gui){

    if(SFX::rocket_launcher_fire_buffer.getDuration() != sf::Time::Zero) return true;

    //  : adjustable rendertx size
    const uint16_t window_height = sf::VideoMode::getDesktopMode().height;
    const uint16_t window_width = sf::VideoMode::getDesktopMode().width;
    renderTexture.create(window_width, window_height);
    
    game_camera.setSize(sf::Vector2u(window_width, window_height));
    ui_camera.setSize(sf::Vector2u(window_width, window_height));
    
    inv.load(world.main_world);
    matUI.load();
    game_camera.zoom(0.99f);
    ui_camera.zoom(1.0f);
    ui_camera.setCenterPosition(sf::Vector2f(0.f, 0.f));

    game_camera.setLeapSpeed(1.f);
    game_camera.setCameraMode(CameraMode::Leap);
    game_camera.setZoomLimited(true);

    Controls::searchForDevices();

    if(!world.init(s))
        perror("VoxelManager failed to load world");
    if(!bg.load()) 
        perror("Background failed to load");
    if(!effOverlay.load()) 
        perror("Effect Overlay failed to load");
    if(!player.load()) 
        perror("Player failed to load");
    if(!SFX::load())
        perror("Failed to load sound effect");
    if(!BGMusic::load())
        perror("Failed to load background music");
    if(!uiStateManager.load(gui, inv, world.main_world))
        perror("Failed to load user interface");
    if(!cursor.load(gui)) 
        perror("Failed to load cursor");
    if(!shaderEffect.load(window_width, window_height)) 
        perror("Failed to load bloom effect");

    return true;
}

void GameState::update()
{
    // Update game status and delta time
    GameStatus::updateBrightness();
    delta_T = deltaClock.restart().asMilliseconds();

    // Handle explosion effects
    if (!world.main_world.explosion_points.empty()) {
        effOverlay.effect_explosion(world.main_world.explosion_points.back());
        world.main_world.explosion_points.pop_back();
    }

    for (auto &wrl : world.add_worlds) {
        if (!wrl.explosion_points.empty()) {
            effOverlay.effect_explosion(wrl.explosion_points.back());
            wrl.explosion_points.pop_back();
        }
    }

    // Update various game components
    bg.update(game_camera.getCenterPosition());
    BGMusic::update();
    effOverlay.update(game_camera.getCenterPosition());
    game_camera.update(delta_T);
    player.update(delta_T);
    matUI.update(world.main_world);
    shaderEffect.update();
    uiStateManager.update(Controls::windowCursorPos);
    world.update();

    // Update inventory item and handle player collisions
    inv.getCurrentItem()->update(world, Controls::worldCursorPos, player.getPhysicsComponent().transform_position, delta_T);
    world.handleCollisionsWithPlayer(player);

    // Update game camera zoom and cursor position
    Controls::gameCameraCenterPos = game_camera.getCenterPosition();
    
    if (Controls::zoomin()) {
        game_camera.zoom(0.99);
    }
    if (Controls::zoomout()) {
        game_camera.zoom(1.01);
    }
}

void GameState::input(sf::Event &ev) {

    // Toggle slow motion (set delta time to 0.1)
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::F)) slowmo = true; // Slow-mo
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)) slowmo = false; // Slow-mo

    uiStateManager.input(ev);

    Controls::setWindowMouseposition(ev);


    if(!GUIfocusedOnObject) {

        if(Controls::useItem(ev)) {
            inv.use(player.getPhysicsComponent().transform_position, Controls::worldCursorPos, world);    
        }
        if(Controls::switchItem(ev)) {
            inv.switchItem();
        }
    }

    if(ev.type == sf::Event::KeyReleased) {
        if(ev.key.code == sf::Keyboard::Escape) {
            statexit();
            GameState::currentState = GameState::menuState;
            //menuState->load("");
        }
    }

}

void GameState::statexit()
{
    BGMusic::stop();
    world.save();
}

void GameState::draw(sf::RenderWindow &window, tgui::BackendGui& gui)
{    
    GUIfocusedOnObject = gui.getFocusedChild() != nullptr;
    // Clear renderTexture
    renderTexture.clear();

    game_camera.setViewTo(renderTexture);

    Controls::setWorldMouseposition(renderTexture);

    game_camera.setTarget(player.getPhysicsComponent().transform_position);

    bg.render(renderTexture);
    player.draw(renderTexture);
    world.render(renderTexture, game_camera.getCenterPosition());
    inv.render(renderTexture);
    
    effOverlay.render(renderTexture);
    
    // Display, draw and set shader
    
    renderTexture.display();
    renderSprite.setTexture(renderTexture.getTexture());


    shaderEffect.render(window, renderSprite);


    // UI Render
    window.setView(window.getDefaultView());

    matUI.render(window);
    uiStateManager.render(window, gui);
    inv.renderUI(window);

    cursor.draw(window);
}
