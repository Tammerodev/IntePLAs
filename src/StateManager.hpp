#pragma once

#include <SFML/Graphics.hpp>

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/CheckBox.hpp>

#include "State.hpp"

#include "Menustate.hpp"
#include "GameState.hpp"
#include "SettingsState.hpp"

MainState* MainState::menuState = new MenuState();
MainState* MainState::gameState = new GameState();
MainState* MainState::settingsState = new SettingsState();
MainState* MainState::currentState = MainState::menuState;

class StateManager {
public:
    void createWindow(uint16_t height, uint16_t width, const std::string &title, bool fullscreen, uint16_t fps_limit) {
        window.create(sf::VideoMode(width,height), title);

        window.setFramerateLimit(fps_limit);
        window.setVerticalSyncEnabled(true);
        window.setVisible(false);

        game_GUI.setWindow(window);
    }
    void init() {

    }
    void gameLoop(){
        MainState::currentState->load("");
        window.setVisible(true);
        while(window.isOpen()) {
            MainState::currentState->update();

            while(window.pollEvent(wevent)) {

                game_GUI.handleEvent(wevent);

                if(wevent.type == sf::Event::Closed) {
                    window.close();
                }
                MainState::currentState->input(wevent);
            }
            
            MainState::currentState->draw(window);

            game_GUI.draw();
            
            window.display();
        }

        MainState::currentState->statexit();
    }
private:
    tgui::Gui game_GUI;
    sf::RenderWindow window;
    sf::Event wevent;
};