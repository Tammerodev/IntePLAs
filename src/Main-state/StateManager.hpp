#pragma once

#include <SFML/Graphics.hpp>

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
        // TODO fullscreen
        if(fullscreen) {
            window.create(sf::VideoMode(height,width), title, sf::Style::Fullscreen);
        } else {
            window.create(sf::VideoMode(height,width), title);
        }
        window.setFramerateLimit(fps_limit);
    }
    void init() {

    }
    void gameLoop(){
        MainState::currentState->load();
        while(window.isOpen()) {
            MainState::currentState->update();

            while(window.pollEvent(wevent)) {
                if(wevent.type == sf::Event::Closed) {
                    window.close();
                }
                MainState::currentState->input(wevent);
            }
            
            MainState::currentState->draw(window);
            window.display();
        }
    }
private:
    sf::RenderWindow window;
    sf::Event wevent;
    sf::RenderTexture renderTexture;
    sf::Sprite renderSprite;
};