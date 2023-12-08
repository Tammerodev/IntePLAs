#include "Game.hpp"

const std::string Game::load(const std::string s, tgui::BackendGui &gui, const int width, const int height) {
    //  : adjustable rendertx size
    const uint16_t window_height = height;
    const uint16_t window_width = width;
    renderTexture.create(window_width, window_height);
    
    game_camera.setSize(sf::Vector2u(window_width, window_height));
    ui_camera.setSize(sf::Vector2u(window_width, window_height));
    ui_camera.setCenterPosition(sf::Vector2f(window_width / 2, window_height / 2));
    
    inv.load(world.main_world);
    matUI.load(gui);
    game_camera.zoom(0.99f);

    

    game_camera.setLeapSpeed(1.f);
    game_camera.setCameraMode(CameraMode::Leap);
    game_camera.setZoomLimited(true);

    Controls::searchForDevices();

    prndd("Initializing worlds...");
    if(!world.init(s))
        perror("VoxelManager failed to load world");

       
    PlayerGlobal::spawn_point = sf::Vector2f((float)(world.main_world.getChunkIndexer().world_sx / 2), -400.f); 

    prndd("Loading background...");
    if(!bg.load()) 
        perror("Background failed to load");
    
    prndd("Loading Effects...");
    if(!effOverlay.load()) 
        perror("Effect Overlay failed to load");

    prndd("Loading player...");
    if(!player.load()) 
        perror("Player failed to load");
    
    
    prndd("Loading sounds...");
    if(!SFX::load())
        perror("Failed to load sound effect");
    if(!BGMusic::load())
        perror("Failed to load background music");

    prndd("Loading UI...");
    if(!uiStateManager.load(gui, inv, world.main_world))
        perror("Failed to load user interface");

    prndd("Compiling shaders...");
    if(!shaderEffect.load(window_width, window_height)) 
        perror("Failed to load bloom effect");

    playerUI.load(window_width, window_height);
    mobManager.load();

    hasLoaded = true;

    // TODO error handling
    return "false";
}

void Game::update() {

    // Update game status and delta time 
    GameStatus::updateBrightness();
    dt = (float)deltaClock.restart().asMilliseconds() / 13.f;

    if(dt > 10.f) {
        dt = 10.f;
    }

    uiStateManager.update(Controls::windowCursorPos);
    shaderEffect.update();
    mobManager.update(dt);
    mobManager.checkCollisions(world.main_world);

    if(PlayerGlobal::health <= 0) return;

    // Handle explosion player damage
    for(const auto point : world.main_world.explosion_points) {
        const sf::Vector2f player_position = sf::Vector2f(player.getPhysicsComponent().transform_position);
        const float distance = math::distance(player_position, point.position);

        if(distance < point.strength) {
            player.damage(point.strength - distance);


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

    for (auto &wrl : world.add_worlds) {
        if (!wrl.explosion_points.empty()) {
            effOverlay.effect_explosion(wrl.explosion_points.back());
            wrl.explosion_points.pop_back();
        }
    }

    mobManager.invokeMobs(player);

    // Update various game components
    bg.update(game_camera.getView(), dt);
    BGMusic::update();
    effOverlay.update(game_camera.getCenterPosition());
    game_camera.update(dt);
    player.update(dt);
    matUI.update(world.main_world);

    // Update UI
    playerUI.update(player);

    world.update(dt, player);

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
}

void Game::render(sf::RenderWindow &window, tgui::BackendGui &gui) {
    window.setView(window.getDefaultView());
    GUIfocusedOnObject = gui.getFocusedChild() != nullptr;

    Display::window_width = window.getSize().x;
    Display::window_height = window.getSize().y;


    // Clear renderTexture
    renderTexture.clear(sf::Color(GameStatus::brightness * 100, GameStatus::brightness * 100, GameStatus::brightness * 100, 255));


    game_camera.setViewTo(renderTexture);

    Controls::setWorldMouseposition(renderTexture);
    
    game_camera.setTarget(sf::Vector2f(player.getPhysicsComponent().transform_position));

    bg.render(renderTexture);

    player.draw(renderTexture);
    mobManager.render(renderTexture);

    world.render(renderTexture, game_camera.getCenterPosition());
    inv.render(renderTexture);

    effOverlay.render(renderTexture);
    
    // Display, draw and set shader
    
    renderTexture.display();
    renderSprite.setTexture(renderTexture.getTexture());


    shaderEffect.render(window, renderSprite);


    // UI Render
    ui_camera.setViewTo(window);


    matUI.render(window);
    uiStateManager.render(window, gui);
    inv.renderUI(window);

    playerUI.render(window);
                
    gui.draw();

    game_camera.setViewTo(renderTexture);
}

void Game::input(sf::Event& ev) {
    // Toggle slow motion (set delta time to 0.1)
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::F)) slowmo = true; // Slow-mo
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)) slowmo = false; // Slow-mo

    uiStateManager.input(ev);

    Controls::setWindowMouseposition(ev);


    if(!GUIfocusedOnObject) {

        if(Controls::useItem(ev)) {
            inv.use(sf::Vector2f(player.getPhysicsComponent().transform_position), Controls::worldCursorPos, world);    
        }

        inv.input(ev);
    }

    if(ev.type == sf::Event::Resized) {
        // Resize
        game_camera.getView().setSize(Display::window_width, Display::window_height);
    }
}
