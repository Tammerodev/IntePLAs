#pragma once
#include "UIState.hpp"
#include "Panel.hpp"
#include "Button.hpp"
#include "cmath"

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/CheckBox.hpp>
#include <TGUI/Widgets/Panel.hpp>


class ItemCreator : public UIState {
public:

    ItemCreator() {

    }

    bool load(tgui::BackendGui& gui) {
        try{
			tgui::Theme theme = tgui::Theme("res/themes/nanogui.style");

            auto panel = tgui::Panel::create();
            panel->setSize(tgui::Layout2d(1500, 600));
            panel->setPosition(tgui::Layout2d(150, 160));
            panel->setRenderer(theme.getRenderer("Panel"));

            auto label_title = tgui::Label::create("New blueprint");
            label_title->setPosition(tgui::Layout2d((150 + 1500) / 2, 200));
            label_title->setRenderer(theme.getRenderer("Label"));

            auto button_exit = tgui::Button::create("exit");
            button_exit->setRenderer(theme.getRenderer("Button"));
            button_exit->onClick(exitbuttonCallback, std::ref(gui));

            // Configure button
            button_exit->setSize(tgui::Layout2d(48 * 4,16 * 4));
            button_exit->setPosition(tgui::Layout2d(200,200));

            gui.add(panel);
            gui.add(button_exit);
            gui.add(label_title);
            

        } catch(std::exception &ex) {
            prnerr("Error at ItemCreator UI state. Could not load gui", ex.what());
        }
        return true;
    }

    void update(const sf::Vector2f &mousepos) {

    }

    void input(sf::Event &e) {

    }

    void draw(sf::RenderTarget& target, tgui::BackendGui& gui) {        
        gui.draw();
    }

    void statexit() {

    }

private:

    static void exitbuttonCallback(tgui::BackendGui& gui) {
        gui.removeAllWidgets();

        UIState::currentState = UIState::nostate;
        UIState::currentState->load(gui);
    }

};