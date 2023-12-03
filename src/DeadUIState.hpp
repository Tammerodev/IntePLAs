#pragma once
#include "UIState.hpp"
#include "PlayerUI.hpp"

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/CheckBox.hpp>

#include "WidgetManager.hpp"

class DeadUIState : public UIState {
public:

    DeadUIState() {

    }

    bool load(tgui::BackendGui& gui, Inventory& inv, VoxelManager& vx) {
        this->gui = &gui;
        this->inv = &inv;
        this->vx = &vx;

        tgui::Theme theme;
        theme.load("res/themes/nanogui.style");

        auto death_message_text = tgui::Label::create("You died!");
        death_message_text->setPosition(Display::window_width / 2, Display::window_height / 2);
        death_message_text->setRenderer(theme.getRenderer("Label"));

        // Origin values from 0 to 1... 0.5 is middle
        death_message_text->setOrigin(0.5, 0.5);

        gui.add(death_message_text);

        auto continue_button = tgui::Button::create("Respawn");
        continue_button->setOrigin(0.5, 0.5);
        continue_button->setPosition(death_message_text->getPosition() + tgui::Layout2d(0, 100));
        continue_button->onClick(respawn, std::ref(gui));

        gui.add(continue_button);

        return true;
    }

    void update(const sf::Vector2f&) {

    }

    void input(sf::Event &e) {
        if(gui == nullptr) return;
        if(inv == nullptr) return;
        if(vx == nullptr)  return;
    }

    void draw(sf::RenderTarget& target, tgui::BackendGui& gui) {
        
    }

    void statexit() {

    }

private:

    tgui::BackendGui* gui = nullptr;
    Inventory *inv = nullptr;
    VoxelManager* vx = nullptr;

    static void respawn(tgui::BackendGui& gui) {
        PlayerGlobal::respawn();
        removeWidgets(gui);
    }
};