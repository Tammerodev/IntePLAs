#include "GameState.hpp"
#include "common.hpp"

bool GameState::load(const std::string s){

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
    if(!uiStateManager.load())
        perror("Failed to load user interface");
    // load only the vertex shader
    if(!shader.loadFromMemory(shader_vert, sf::Shader::Vertex));
    // load only the fragment shader
    if(!shader.loadFromMemory(shader_frag, sf::Shader::Fragment));
    // load both shaders
    if(!shader.loadFromMemory(shader_vert, shader_frag));

    return true;
}

void GameState::update()
{
    const sf::Vector2f mousepos = sf::Vector2f(sf::Mouse::getPosition());

    delta_T = deltaClock.getElapsedTime().asMilliseconds();
    if(slowmo) delta_T = 0.2f;
    deltaClock.restart();

    shader.setUniform("time",shader_time.getElapsedTime().asSeconds());
    shader.setUniform("resolution",sf::Vector2(1.0f,1.0f));


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


    uiStateManager.update(mousepos);

    world.update();
        
    inv.getCurrentItem()->update(world, mousepos, player.getPhysicsComponent().transform_position, delta_T);

    world.handleCollisionsWithPlayer(player);
}

void GameState::input(sf::Event &ev) {

    // Toggle slow motion (set delta time to 0.1)
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::F)) slowmo = true; // Slow-mo
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)) slowmo = false; // Slow-mo

    if(Controls::useItem(ev)) {
        inv.use(player.getPhysicsComponent().transform_position, sf::Vector2f(renderTexture.mapPixelToCoords(sf::Mouse::getPosition())), world);    
    }
    if(Controls::switchItem(ev)) {
        inv.switchItem();
    }


    if(ev.type == sf::Event::KeyReleased) {
        if(ev.key.code == sf::Keyboard::G) {
            inv.addItem(world.main_world, std::make_shared<ItemCreateItem>(world.main_world));
            prndd("Added item");
        }

        if(ev.key.code == sf::Keyboard::Escape) {
            statexit();
            GameState::currentState = GameState::menuState;
            menuState->load("");
        }
    }

    if(Controls::zoomin())
        game_camera.zoom(0.99);
    if(Controls::zoomout())
        game_camera.zoom(1.01);

    uiStateManager.input(ev);
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
    game_camera.setTarget(player.getPhysicsComponent().transform_position);

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

        // UI Render
        ui_camera.setViewTo(renderTexture);
        matUI.render(renderTexture);
        uiStateManager.render(renderTexture);

    game_camera.setViewTo(renderTexture);
    
    // Display, draw and set shader
    renderTexture.display();
    renderSprite.setTexture(renderTexture.getTexture());
    window.draw(renderSprite , &shader);

    float currentTime = clock.restart().asMilliseconds();
    float fps = 1.f / (currentTime - lastTime);
    lastTime = currentTime;
}
