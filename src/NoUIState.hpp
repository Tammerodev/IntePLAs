#pragma once
#include "UIState.hpp"

class NoUIState : public UIState {
public:

    NoUIState() {

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

};