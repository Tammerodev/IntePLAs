#pragma once

#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/CheckBox.hpp>

#include "State.hpp"
#include "Menustate.hpp"
#include "GameState.hpp"
#include "SettingsState.hpp"
#include "LoadingState.hpp"
#include "ServerHostState.hpp"
#include "WorldSelectionState.hpp"

MainState* MainState::menuState = new MenuState();
MainState* MainState::gameState = new GameState();
MainState* MainState::settingsState = new SettingsState();
MainState* MainState::loadState = new Loading();
MainState* MainState::serverHostState = new ServerHostState();
MainState* MainState::worldSelectionState = new WorldSelectionState();

MainState* MainState::currentState = MainState::menuState;

class StateManager {
public:
    void createWindow(uint16_t height, uint16_t width, const std::string &title, bool fullscreen, uint16_t fps_limit) {
        if(fullscreen) window.create(sf::VideoMode(width,height), title, sf::Style::Fullscreen);
        else window.create(sf::VideoMode(width,height), title);

        window.setFramerateLimit(fps_limit);
        window.setVisible(false);

        gui.setWindow(window);
    }

    void init() {
        SettingsLoader::loadSettings();
    }

    void gameLoop() {
        MainState::currentState->load("", gui);
        window.setVisible(true);


    Display::window_width = window.getSize().x;
    Display::window_height = window.getSize().y;

        while(window.isOpen()) {
            MainState::currentState->update();

            {
                
                std::lock_guard<std::mutex> lock(windowMutex);

                Display::window_width = window.getSize().x;
                Display::window_height = window.getSize().y;

                sf::Event testevent = MainState::currentState->event_sig();

                window.setFramerateLimit(GraphicsSettings::max_fps);
                window.setVerticalSyncEnabled(GraphicsSettings::use_vsync);

                gui.handleEvent(testevent);

                while(window.pollEvent(wevent)) {
                    gui.handleEvent(wevent);

                    MainState::currentState->input(wevent);
                }

                MainState::currentState->draw(window, gui);
                ++Globals::frame;
            }

            {
                std::lock_guard<std::mutex> lock(guiMutex);
            }

            window.display();
        }
    }

private:
    tgui::Gui gui;
    sf::RenderWindow window;
    sf::Event wevent;
    std::mutex windowMutex;  // Mutex for sf::RenderWindow
    std::mutex guiMutex;     // Mutex for tgui::Gui
};
