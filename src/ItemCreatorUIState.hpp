#pragma once
#include "UIState.hpp"
#include "Panel.hpp"
#include "Button.hpp"

class ItemCreator : public UIState {
public:

    ItemCreator() {
        panel = Panel("res/img/UI/Blueprint.png");
        tex.loadFromFile("res/img/UI/Blueprint.png");
        cancelButton = Button(tex, sf::Vector2f(213, 28), sf::Vector2f(1.0f, 1.0f), sf::IntRect(213, 28, 11, 11));
    }

    bool load() {
        return true;
    }

    void update() {
        
    }

    void input(sf::Event &e) {
        if(cancelButton.getState(sf::Vector2f(e.mouseMove.x, e.mouseMove.y)) == Button::ButtonState::Click) {
            UIState::currentState = UIState::nostate;
            UIState::currentState->load();
        }
    }

    void draw(sf::RenderTarget& target) {
        panel.applyTexture();
        target.draw(panel);
        cancelButton.setTexture(tex);
        target.draw(cancelButton);
    }

    void statexit() {

    }

private:

    Panel panel;
    sf::Texture tex;
    Button cancelButton;

};