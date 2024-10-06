#pragma once
#include "UIState.hpp"
#include "PlayerUI.hpp"

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/CheckBox.hpp>

#include "WidgetManager.hpp"
#include "GameStatus.hpp"

class NoUIState : public UIState {
public:

    NoUIState() {

    }

    bool load(tgui::BackendGui& gui, Inventory& inv, VoxelManager& vx) {
        this->gui = &gui;
        this->inv = &inv;
        this->vx = &vx;
    
        voxelLabel = tgui::Label::create("");
        voxelLabel->setPosition(tgui::Layout2d(250, 0));
        voxelLabel->setSize(tgui::Layout2d(300, 24));

        timeLabel = tgui::Label::create("");
        timeLabel->setPosition(tgui::Layout2d(250, 34));
        timeLabel->setSize(tgui::Layout2d(300, 24));

        gui.add(voxelLabel);
        gui.add(timeLabel);
        
        return true;
    }

    void update(const sf::Vector2f&) {
        const std::string timeText = "Time : " + std::to_string(GameStatus::brightness);
        timeLabel->setText(timeText);
    }

    void input(sf::Event &e) {
        if(gui == nullptr) return;
        if(inv == nullptr) return;
        if(vx == nullptr)  return;

        if(Controls::pause(e)) {
            exit_to_pause(*gui, *inv, *vx);
        }

        Voxel &voxel = vx->getChunkIndexer().boundGetVoxelAt(Controls::worldCursorPos.x, Controls::worldCursorPos.y);

        const std::string voxelName = elm::getInfoFromType(voxel.value).name;
        const std::string tempatature = "\t" + std::to_string(voxel.temp);
        const std::string voltage = "\t" + std::to_string(voxel.voltage) + "v";

        //voxelLabel->setVisible(voxelName != "");
        voxelLabel->setText(voxelName + tempatature + voltage);

    }

    void draw(sf::RenderTarget& target, tgui::BackendGui& gui) {

    }

    void statexit() {

    }

private:

    tgui::BackendGui* gui = nullptr;
    Inventory *inv = nullptr;
    VoxelManager* vx = nullptr;

    tgui::Label::Ptr voxelLabel = nullptr; 
    tgui::Label::Ptr timeLabel = nullptr; 

    void exit_to_itemcreator(tgui::BackendGui& gui, Inventory &inv, VoxelManager& vx) {
        removeWidgets(gui);

        UIState::currentState = UIState::itemcreator;
        UIState::currentState->load(gui, inv, vx);
    }

    void exit_to_pause(tgui::BackendGui& gui, Inventory &inv, VoxelManager& vx) {
        removeWidgets(gui);

        UIState::currentState = UIState::pause;
        UIState::currentState->load(gui, inv, vx);
    }
};