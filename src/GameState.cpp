#include "GameState.hpp"
#include "common.hpp"
#include "LoadingScreen.hpp"

static void mul_load(const std::string s, tgui::BackendGui& gui, Game& game, const int width, const int height) {
    game.load(s, gui, width, height);
}

bool GameState::load(const std::string s, tgui::BackendGui& gui) {
    path = s;
    return true;
}

void GameState::update() {
    if(game.getLoaded())
        game.update();
}

void GameState::input(sf::Event &ev) {
    if(game.getLoaded())
        game.input(ev);
}

void GameState::statexit() {
    BGMusic::stop();
    game.exit();
}

void GameState::draw(sf::RenderWindow &window, tgui::BackendGui& gui) {  
    if(game.getLoaded()) {
        game.render(window, gui);
    }
    else {
        const int width = sf::VideoMode::getDesktopMode().width;
        const int height = sf::VideoMode::getDesktopMode().height;

        // Deactivate window on main thread
        window.setActive(false);
        
        // Start thread for loading screen
        std::thread load_screen_thread(LoadingScreen::load_screen, std::ref(window), std::ref(game), window.getSize());

        bool success = false;

        game.load(path, gui, width, height);

        // Wait for load screen to stop
        load_screen_thread.join();

        // Reactivate window on main thread
        window.setActive(true);
    }
}
