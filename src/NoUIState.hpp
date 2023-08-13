#pragma once
#include "UIState.hpp"

class NoUIState : public UIState {
public:

    NoUIState() {

    }

    bool load() {
        return true;
    }

    void update(const sf::Vector2f&) {

    }

    void input(sf::Event &e) {

    }

    void draw(sf::RenderTarget& target) {

    }

    void statexit() {

    }

private:

};