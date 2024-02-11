#pragma once
#include "UIState.hpp"
#include "PlayerUI.hpp"

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/CheckBox.hpp>

#include "WidgetManager.hpp"
#include "Globals.hpp"


class PhotoModeUIState : public UIState {
public:

    PhotoModeUIState() {

    }

    bool load(tgui::BackendGui& gui, Inventory& inv, VoxelManager& vx) {
        this->gui = &gui;
        this->inv = &inv;
        this->vx = &vx;

        shoot = tgui::Button::create("Take picture");
        shoot->setPosition(tgui::Layout2d(250, 10));
        shoot->setSize(tgui::Layout2d(100, 24));

        gui.add(shoot);

        shootTimer = shootTime;
        
        return true;
    }

    void update(const sf::Vector2f&) {

    }

    void input(sf::Event &e) {
        if(gui == nullptr) return;
        if(inv == nullptr) return;
        if(vx == nullptr)  return;

        if(Controls::pause(e)) exit(*gui, *inv, *vx);

        if(shoot->isMouseDown()) {
            readyToShoot = true;
        }

    }

    void draw(sf::RenderTarget& target, tgui::BackendGui& gui) {
        debug_globals::photoMode = true;

        if(readyToShoot) {
            shootTimer--;

            if(shootTimer <= 0) {
                sf::Texture texture;
                texture.create(target.getSize().x, target.getSize().y);

                if(Globals::renderSprite == nullptr) return;

                texture = *(sf::Texture*)Globals::renderSprite->getTexture();
                texture.copyToImage().saveToFile(StorageSettings::screenshot_path + std::to_string(time(0)) + ".png");

                readyToShoot = false;
                shootTimer = shootTime;
            }
        }

        shoot->setVisible(!readyToShoot);

        gui.draw();
    }

    void statexit() {
        debug_globals::photoMode = false;
    }

private:
    bool readyToShoot = false;
    const int shootTime = 50;
    int shootTimer = 0;

    tgui::Button::Ptr shoot = nullptr;

    tgui::BackendGui* gui = nullptr;
    Inventory *inv = nullptr;
    VoxelManager* vx = nullptr;

    void exit(tgui::BackendGui& gui, Inventory &inv, VoxelManager& vx) {
        removeWidgets(gui);
        statexit();

        UIState::currentState = UIState::nostate;
        UIState::currentState->load(gui, inv, vx);
    }
};