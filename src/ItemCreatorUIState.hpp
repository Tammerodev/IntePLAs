#pragma once
#include "UIState.hpp"
#include "Panel.hpp"
#include "Button.hpp"

class ItemCreator : public UIState {
public:

    ItemCreator() {
        edit_img.create(16,16,sf::Color::Red);
        edit_tex.create(16,16);
    }

    bool load() {
        return true;
    }

    void update(const sf::Vector2f &mousepos) {
        edit_tex.update(edit_img);
        edit_spr.setTexture(edit_tex);
        edit_spr.setPosition(100,100);
        edit_spr.setScale(10.f, 10.f);

    }

    void input(sf::Event &e) {

    }

    void draw(sf::RenderTarget& target) {
        target.draw(edit_spr);
    }

    void statexit() {

    }

private:

    sf::Image edit_img;
    sf::Texture edit_tex;
    sf::Sprite edit_spr;

};