#include "GameState.hpp"
#include "common.hpp"
#include "LoadingScreen.hpp"
#include "SavingScreen.hpp"

static void mul_load(const std::string s, tgui::BackendGui& gui, Game& game, const int width, const int height) {
    game.load(s, gui, width, height);
}

bool GameState::load(const std::string s, tgui::BackendGui& gui) {
    path = s;
    Globals::resetFrame();
    return true;
}

void GameState::update() {
    if(game.getLoaded()) {
        game.update();
        for(auto &mod : modsInUse) {
            mod->update(game);
        }
    }
}

void GameState::input(sf::Event &ev) {
    if(ev.type == sf::Event::Closed) {
        game.exit();
    }

    if(game.getLoaded())
        game.input(ev);
}

void GameState::statexit() {
    if(gui_ptr == nullptr) return;
    if(window_ptr == nullptr) return;

    prndd("Saving the game...");


    const int width = sf::VideoMode::getDesktopMode().width;
    const int height = sf::VideoMode::getDesktopMode().height;

    // Deactivate window on main thread
    window_ptr->setActive(false);
    
    // Start thread for loading screen
    std::thread save_screen_thread(SavingScreen::load_screen, window_ptr, std::ref(game), window_ptr->getSize());

    game.saveWorld();

    game.hasSaved = true;

    prndd("World saved, waiting for thread to stop...");

    // Wait for load screen to stop
    save_screen_thread.join();
    
    // Reactivate window on main thread
    window_ptr->setActive(true);

    window_ptr->close();
}

void GameState::draw(sf::RenderWindow &window, tgui::BackendGui& gui) {  

    Display::window_width = window.getSize().x;
    Display::window_height = window.getSize().y;

    if(game.getLoaded()) {
        window_ptr = &window;
        gui_ptr = &gui;

        game.renderFirst(window, gui);

        for(const auto &mod : modsInUse) {
            mod->render(game.getRenderTexture(), game, gui);
        }

        game.renderLast(window, gui);


        // Hide cursor
        window.setMouseCursorVisible(true);
    }
    else {
        const int width = sf::VideoMode::getDesktopMode().width;
        const int height = sf::VideoMode::getDesktopMode().height;

        modsInUse.push_back(std::make_shared<HeistMod>());


        cursor.load(gui, window);

        // Deactivate window on main thread
        window.setActive(false);
        
        // Start thread for loading screen
        std::thread load_screen_thread(LoadingScreen::load_screen, std::ref(window), std::ref(game), window.getSize());

        bool success = false;

        game.load(path, gui, width, height);

        for(const auto &mod : modsInUse) {
            mod->load(game, gui);
        } 

        // Wait for load screen to stop
        load_screen_thread.join();

        // Reactivate window on main thread
        window.setActive(true);
    }
}
