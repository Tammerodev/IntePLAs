#pragma once
#include "UIState.hpp"
#include "PlayerUI.hpp"

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/CheckBox.hpp>

class NoUIState : public UIState {
public:

    NoUIState() {

    }

    bool load(tgui::BackendGui& gui, Inventory& inv, VoxelManager& vx) {
        try{
			tgui::Theme theme = tgui::Theme("res/themes/nanogui.style");

			auto itemcreatorButton = tgui::Button::create("Blueprint creator");
            itemcreatorButton->setRenderer(theme.getRenderer("Button"));
            itemcreatorButton->onClick(exitbuttonCallback, std::ref(gui), std::ref(inv), std::ref(vx));
            itemcreatorButton->setPosition(10,10);

            gui.add(itemcreatorButton);
            

        } catch(std::exception &ex) {
            prnerr("Error at ItemCreator UI state. Could not load gui", ex.what());
            return false;
        }
        return true;
    }

    void update(const sf::Vector2f&) {

    }

    void input(sf::Event &e) {

    }

    void draw(sf::RenderTarget& target, tgui::BackendGui& gui) {

    }

    void statexit() {

    }

private:


    static void exitbuttonCallback(tgui::BackendGui& gui, Inventory &inv, VoxelManager& vx) {
        removeWidgets(gui);

        UIState::currentState = UIState::itemcreator;
        UIState::currentState->load(gui, inv, vx);
    }
};