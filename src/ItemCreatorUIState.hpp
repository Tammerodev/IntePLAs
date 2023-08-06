#pragma once
#include "UIState.hpp"
#include "Panel.hpp"

class ItemCreator : public UIState {
public:

    ItemCreator() {
        panel = Panel("res/img/UI/Blueprint.png");
    }

    bool load() {
        return true;
    }

    void update() {

    }

    void input(sf::Event &e) {

    }

    void draw(sf::RenderTarget& target) {

    }

    void statexit() {

    }

private:

    Panel panel;

};