#include "Game.hpp"

const std::string Game::load(const std::string s, tgui::BackendGui &gui, const int width, const int height) {
    //  : adjustable rendertx size
    const uint16_t window_height = height;
    const uint16_t window_width = width;

    local_gui = &gui;

    load_state::setState(load_state::Pre_initialization);

    renderTexture.create(window_width, window_height);

    Fonts::loadFont();
    
    game_camera.setSize(sf::Vector2u(window_width, window_height));
    ui_camera.setSize(sf::Vector2u(window_width, window_height));
    ui_camera.setCenterPosition(sf::Vector2f(window_width / 2, window_height / 2));
    
    inv.load(world.main_world);
    matUI.load(gui);
    game_camera.zoom(0.60f);
    

    game_camera.setLeapSpeed(1.f);
    game_camera.setCameraMode(CameraMode::Leap);
    game_camera.setZoomLimited(true);

    load_state::setState(load_state::Search_devices);

    Controls::searchForDevices();

    load_state::setState(load_state::Initializing_map);
    if(!world.init(s))
        perror("VoxelManager failed to load world");

       
    PlayerGlobal::spawn_point = sf::Vector2f((float)(worldSize::world_sx / 2), -400.f); 

    load_state::setState(load_state::Loading_sprites);
    if(!bg.load()) 
        perror("Background failed to load");
    
    load_state::setState(load_state::Loading_effects);
    if(!effOverlay.load()) 
        perror("Effect Overlay failed to load");

    load_state::setState(load_state::Loading_player);
    if(!player.load()) 
        perror("Player failed to load");
    
    
    load_state::setState(load_state::Loading_sounds);
    if(!SFX::load())
        perror("Failed to load sound effect");

    load_state::setState(load_state::Loading_sounds);
    if(!BGMusic::load())
        perror("Failed to load background music");

    load_state::setState(load_state::Loading_ui);
    if(!uiStateManager.load(gui, inv, world.main_world))
        perror("Failed to load user interface");

    load_state::setState(load_state::Compiling_shaders);
    if(!shaderEffect.load(window_width, window_height)) 
        perror("Failed to load bloom effect");

    load_state::setState(load_state::Loading_mobs);
    // TODO
    load_state::setState(load_state::Spawning_mobs);
    // TODO

    load_state::setState(load_state::Finishing);

    gameEventManager.load();

    playerUI.load(window_width, window_height);

    debugDisplay.load(gui);


    hasLoaded = true;

    prndd("Everything loaded!");

    // TODO error handling
    return "false";
}

void Game::update() {
    if(goingToSave) exit();

    // Update game status and delta time 
    dt = (float)deltaClock.restart().asMilliseconds() / 13.f;

    debug_globals::frame_time = dt;

    if(dt > 10.f) {
        dt = 10.f;
    }

    uiStateManager.update(Controls::windowCursorPos);
    shaderEffect.update();
    gameEventManager.update(world.main_world);
    
    if(PlayerGlobal::health <= 0) return;                 

    for (auto &wrl : world.add_worlds) {
        if (!wrl.explosion_points.empty()) {
            effOverlay.effect_explosion(wrl.explosion_points.back());
            wrl.explosion_points.pop_back();
        }
    }

    // Update various game components
    bg.update(game_camera.getView(), dt);
    bg.updateForBiomeChanges(player, world.main_world);

    BGMusic::update();
    effOverlay.update(game_camera.getCenterPosition());
    game_camera.update(dt);
    player.update(world.main_world.getChunkIndexer(), dt);
    matUI.update(world.main_world);

    // Update UI
    playerUI.update(player);

    // Update inventory item and handle player collisions
    inv.getCurrentItem()->update(world, Controls::worldCursorPos, sf::Vector2f(player.getPhysicsComponent().transform_position + sf::Vector2f(0, 4)), dt, player);
    world.handleCollisionsWithPlayer(player);

    // Update game camera zoom and cursor position
    Controls::gameCameraCenterPos = game_camera.getCenterPosition();
    
    if (Controls::zoomin()) {
        game_camera.zoom(0.99);
    }
    if (Controls::zoomout()) {
        game_camera.zoom(1.01);
    }


    world.update(dt, player, gameEventManager.getEvent());
    world.mobInvoke(player, world.main_world.explosion_points);

    world.setAddWorldsPointImpact(world.main_world.explosion_points);

    // Handle explosion player damage
    for(const auto point : world.main_world.explosion_points) {
        const sf::Vector2f player_position = sf::Vector2f(player.getPhysicsComponent().transform_position);
        const float distance = math::distance(player_position, point.position);

        if(distance < point.strength * 5) {    
            game_camera.shake(point.strength * 5 - distance);
        }

        if(distance < point.strength) {
            player.getHealthManager().damageHit(point.strength - distance);
            // Launch the player in opposite direction
            player.getPhysicsComponent().setVelocity(
                math::subVector(player_position, point.position) / (distance * player.getPhysicsComponent().weight)
                );
        } 
    }


    // Handle explosion effect
    if (!world.main_world.explosion_points.empty()) {
        effOverlay.effect_explosion(world.main_world.explosion_points.back());
        world.main_world.explosion_points.pop_back();
    }
}

void Game::renderFirst(sf::RenderWindow &window, tgui::BackendGui &gui) {
    GameStatus::updateBrightness();
    

    window.setView(window.getDefaultView());
    GUIfocusedOnObject = gui.getFocusedChild() != nullptr;

    // Clear renderTexture
    renderTexture.clear(sf::Color(GameStatus::brightness * 100, GameStatus::brightness * 100, GameStatus::brightness * 100, 255));
    shaderEffect.clearTextures();

    game_camera.setViewTo(renderTexture);

    Controls::setWorldMouseposition(renderTexture);
    
    game_camera.setTarget(sf::Vector2f(player.getPhysicsComponent().transform_position));

    if(debug_view::forced_view_center != sf::Vector2f(0.f, 0.f)) {
        game_camera.setTarget(debug_view::forced_view_center);
    } 

    bg.render(renderTexture);

    if(!debug_globals::photoMode) {
        player.draw(renderTexture);
        inv.render(renderTexture);
    }
       
    world.render(renderTexture, shaderEffect.getTresholdTexture(), game_camera.getCenterPosition());
}


void Game::renderLast(sf::RenderWindow &window, tgui::BackendGui &gui) {
    
    ui_camera.setViewTo(renderTexture);
    gameEventManager.render(renderTexture);

    game_camera.setViewTo(renderTexture); 
    effOverlay.render(renderTexture);

    renderTexture.display();
    renderSprite.setTexture(renderTexture.getTexture());

    shaderEffect.render(window, renderSprite);      // Shader first render

    game_camera.setViewTo(shaderEffect.getTresholdTexture());
        world.main_world.renderSimulationManager(renderTexture, shaderEffect.getTresholdTexture()); // Simulation manager (lights)
    ui_camera.setViewTo(shaderEffect.getTresholdTexture());

    shaderEffect.finalRender(window);               // Shader second render

    Globals::renderSprite = &renderSprite;
    if(!debug_globals::photoMode) renderUI(window, gui);

    uiStateManager.render(window, gui);


    game_camera.setViewTo(renderTexture);
}

void Game::renderUI(sf::RenderWindow& window, tgui::BackendGui& gui) {
    ui_camera.setViewTo(window);

    matUI.render(window);
    inv.renderUI(window);

    playerUI.render(window);

    debugDisplay.render(window);       

    gui.draw();
}

void Game::input(sf::Event& ev) {
    uiStateManager.input(ev);

    Controls::setWindowMouseposition(ev);

    if(!GUIfocusedOnObject) {

        if(Controls::useItem(ev)) {
            inv.use(player, Controls::worldCursorPos, world);    
        }
        inv.input(ev);
    }

    if(ev.type == sf::Event::Resized) {
        // Resize
        game_camera.getView().setSize(Display::window_width, Display::window_height);
    }

    if(ev.type == sf::Event::KeyReleased) {
        if(ev.key.code == sf::Keyboard::Key::L) {
            SettingsLoader::loadSettings();
            shaderEffect.load(Display::window_width, Display::window_height);
        }
    }
}
