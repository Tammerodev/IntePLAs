#pragma once
#include "UIState.hpp"
#include "PlayerUI.hpp"

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/CheckBox.hpp>

#include "WidgetManager.hpp"


class SaveExitUIState : public UIState {
public:

    SaveExitUIState() {

    }

    bool load(tgui::BackendGui& gui, Inventory& inv, VoxelManager& vx) {
        this->gui = &gui;
        this->inv = &inv;
        this->vx = &vx;

		tgui::Theme theme = tgui::Theme("res/themes/nanogui.style");

        auto panel = tgui::Panel::create();
        panel->setPosition(Display::window_width / 2, Display::window_height / 2);
        panel->setSize(500, 400);
        panel->setPosition(panel->getPosition() - panel->getSize() / 2);

        saveNameEditBox = tgui::EditBox::create();
        saveAndExitButton = tgui::Button::create("Save and Exit");
        cancelSaveButton = tgui::Button::create("Cancel");
        
        panel->setRenderer(theme.getRenderer("Panel"));
        saveNameEditBox->setRenderer(theme.getRenderer("EditBox"));
        saveAndExitButton->setRenderer(theme.getRenderer("Button"));
        cancelSaveButton->setRenderer(theme.getRenderer("Button"));

        saveNameEditBox->setSize(100, 25);
        saveNameEditBox->setPosition((Display::window_width / 2) - saveNameEditBox->getSize().x, Display::window_height / 2);

        saveAndExitButton->setSize(70, 25);
        cancelSaveButton->setSize(70, 25);

        saveAndExitButton->setPosition((Display::window_width / 2) + 120, Display::window_height / 1.5);
        cancelSaveButton->setPosition((Display::window_width / 2) - (120 + cancelSaveButton->getSize().x), Display::window_height / 1.5);

        gui.add(panel);
        gui.add(saveNameEditBox);
        gui.add(saveAndExitButton);
        gui.add(cancelSaveButton);

        return true;
    }

    void update(const sf::Vector2f&) {
        
    }

    void input(sf::Event &e) {
        if(gui == nullptr) return;
        if(inv == nullptr) return;
        if(vx == nullptr)  return;

        if(saveAndExitButton->isMouseDown())
            exit_to_save(*gui, *inv, *vx);
        if(saveAndExitButton->isMouseDown())
            exit_to_pause(*gui, *inv, *vx);
        
    }

    void draw(sf::RenderTarget& target, tgui::BackendGui& gui) {

    }

    void statexit() {

    }

private:

    tgui::EditBox::Ptr saveNameEditBox  = nullptr;
    tgui::Button::Ptr saveAndExitButton = nullptr;
    tgui::Button::Ptr cancelSaveButton  = nullptr;

    tgui::BackendGui* gui = nullptr;
    Inventory *inv = nullptr;
    VoxelManager* vx = nullptr;

    void exit_to_save(tgui::BackendGui& gui, Inventory &inv, VoxelManager& vx) {
        if(saveNameEditBox == nullptr) return;
        removeWidgets(gui);

        Globals::exitSaveName = saveNameEditBox->getText().toStdString();
    }

    void exit_to_pause(tgui::BackendGui& gui, Inventory &inv, VoxelManager& vx) {
        removeWidgets(gui);

        UIState::currentState = UIState::pause;
        UIState::currentState->load(gui, inv, vx);
    }
};