#pragma once
#include "UIState.hpp"
#include "PlayerUI.hpp"

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/CheckBox.hpp>

#include "WidgetManager.hpp"

#include "Palettes/PaletteUI.hpp"

class PauseUIState : public UIState {
public:

    PauseUIState() {

    }

    bool load(tgui::BackendGui& gui, Inventory& inv, VoxelManager& vx) {
        this->gui = &gui;
        this->inv = &inv;
        this->vx = &vx;

        darken_rect = sf::RectangleShape(sf::Vector2f(Display::window_width, Display::window_height));
        darken_rect.setFillColor(sf::Color(Palette::PaletteUI::Gray.r, Palette::PaletteUI::Gray.g, Palette::PaletteUI::Gray.b, 100));

        top_rect = sf::RectangleShape(sf::Vector2f(Display::window_width, 16));
        top_rect.setFillColor(Palette::PaletteUI::WineRed);

        bottom_rect = sf::RectangleShape(sf::Vector2f(Display::window_width, 16));
        bottom_rect.setFillColor(Palette::PaletteUI::White);


        darken_rect.move(sf::Vector2f(-Display::window_width, 0)); 
        top_rect.move(sf::Vector2f(Display::window_width, 0)); 
        bottom_rect.move(sf::Vector2f(Display::window_width, Display::window_height - 16)); 
    
        
        return true;
    }

    void update(const sf::Vector2f&) {
        if(darken_rect.getPosition().x < 0.f) {
            const float movement = 0.f - darken_rect.getPosition().x;
            darken_rect.move(sf::Vector2f(movement / 10.f, 0.f));
        }

        if(top_rect.getPosition().x > 0.f) {
            const float movement = top_rect.getPosition().x;
            top_rect.move(sf::Vector2f(-(movement / 10.f), 0.f));
        }

        if(bottom_rect.getPosition().x > 0.f) {
            const float movement = bottom_rect.getPosition().x;
            bottom_rect.move(sf::Vector2f(-(movement / 10.f), 0.f));
        }
    }   

    void input(sf::Event &e) {
        if(gui == nullptr) return;
        if(inv == nullptr) return;
        if(vx == nullptr)  return;

        if(Controls::pause(e) && abs(darken_rect.getPosition().x) < 20 ) {
            exit(*gui, *inv, *vx);
        }
    }

    void draw(sf::RenderTarget& target, tgui::BackendGui& gui) {
        target.draw(darken_rect);
        target.draw(bottom_rect);
        target.draw(top_rect);

    }

    void statexit() {

    }

private:

    tgui::BackendGui* gui = nullptr;
    Inventory *inv = nullptr;
    VoxelManager* vx = nullptr;

    sf::RectangleShape darken_rect;
    sf::RectangleShape top_rect;
    sf::RectangleShape bottom_rect;


    void exit(tgui::BackendGui& gui, Inventory &inv, VoxelManager& vx) {
        removeWidgets(gui);

        UIState::currentState = UIState::nostate;
        UIState::currentState->load(gui, inv, vx);
    }
};