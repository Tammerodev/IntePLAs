#pragma once
#include "UIState.hpp"
#include "Panel.hpp"
#include "Button.hpp"
#include "cmath"
#include "math.hpp"
#include "Elements.hpp"
#include "PlayerUI.hpp"
#include "WidgetManager.hpp"
#include "SelectMould.hpp"
#include "FinalImage.hpp"

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/CheckBox.hpp>
#include <TGUI/Widgets/Panel.hpp>

namespace FileGlobal {
    inline int mouldIndex = 0;
}

class ItemCreator : public UIState {
public:

    ItemCreator() {

    }

    void loadImages() {
        if(p_gui != nullptr) {
            p_gui->remove(finalPicture);
            p_gui->remove(mouldPicture);
        }

        finalPicture = tgui::Picture::create(finalTx);
        mouldPicture = tgui::Picture::create(mouldTx, true);

        mouldPicture->setSize(100, 100);
        mouldPicture->setPosition(tgui::Layout2d("65%", "60%"));

        finalPicture->setSize(100, 100);
        finalPicture->setPosition(tgui::Layout2d("15%", "45%"));

        if(p_gui != nullptr) {
            p_gui->add(finalPicture);
            p_gui->add(mouldPicture);
        }
    }

    bool load(tgui::BackendGui& gui, Inventory& inv, VoxelManager& vx) {
        p_gui = &gui;

        try{
			tgui::Theme theme = tgui::Theme("res/themes/nanogui.style");
            
            tgui::Texture::setDefaultSmooth(false);
            mouldTx.load("res/img/UI/selectMould.png");

            loadImages();

            auto panel = tgui::Panel::create();
            panel->setSize(tgui::Layout2d("80%","75%"));
            panel->setPosition(tgui::Layout2d("10%", "12.5%"));
            panel->setRenderer(theme.getRenderer("Panel"));

            auto label_title = tgui::Label::create("New blueprint"); 
            label_title->setPosition(tgui::Layout2d((150 + 1500) / 2, 200));
            label_title->setRenderer(theme.getRenderer("Label"));

            auto button_exit = tgui::Button::create("exit");
            button_exit->setRenderer(theme.getRenderer("Button"));
            button_exit->onClick(exitbuttonCallback, std::ref(gui), std::ref(inv), std::ref(vx));

            auto button_build = tgui::Button::create("Build");
            button_build->setPosition(tgui::Layout2d("40%", "40%"));
            button_build->setRenderer(theme.getRenderer("Button"));
            button_build->onClick(buildCallback, std::ref(imageUI), std::ref(savedReturnPath));

            auto button_save = tgui::Button::create("save");
            button_save->setRenderer(theme.getRenderer("Button"));
            button_save->onClick(savebuttonCallback, std::ref(gui), std::ref(savedReturnPath), std::ref(inv), std::ref(vx));
            button_save->setSize(tgui::Layout2d(48 * 4,16 * 4));
            button_save->setPosition(tgui::Layout2d(200,270));

            // Configure button
            button_exit->setSize(tgui::Layout2d(48 * 4,16 * 4));
            button_exit->setPosition(tgui::Layout2d(200,200));

            gui.add(panel);

            gui.add(button_exit);
            gui.add(button_save);
            gui.add(label_title);
            
            gui.add(finalPicture);
            gui.add(mouldPicture);
            gui.add(button_build);

            mouldUI.load(gui);
            imageUI.load(gui, finalPicture->getPosition());

        } catch(std::exception &ex) {
            prnerr("Error at ItemCreator UI state. Could not load gui", ex.what());
        }
        return true;
    }

    void update(const sf::Vector2f &mousepos) {
        imageUI.update();

        if(!savedReturnPath.empty()) {
            finalTx.load(savedReturnPath);

            loadImages();
        }
    }

    void input(sf::Event &e) {  
        if(e.type == sf::Event::MouseButtonPressed) {
            tgui::String s = mouldUI.update(*p_gui);

            if(s != "") {
                int left = std::stoi(s.toStdString());
                mouldTx.load("res/img/Item/moulds.png", tgui::UIntRect(left, 0, 16, 16));

                FileGlobal::mouldIndex = left;

                loadImages();
                mouldUI.hide();
            }

            if(mouldPicture->isMouseDown())
                mouldUI.show(mouldPicture->getPosition());
        }
    }

    void draw(sf::RenderTarget& target, tgui::BackendGui& gui) {        

    }


    void statexit() {

    }

private:
    SelectMouldUI mouldUI;
    FinalImageUI imageUI;

    tgui::Picture::Ptr finalPicture = nullptr;
    tgui::Picture::Ptr mouldPicture = nullptr;

    tgui::Texture mouldTx;
    tgui::Texture finalTx;

    tgui::BackendGui *p_gui = nullptr;

    std::string savedReturnPath = "";

    static void exitbuttonCallback(tgui::BackendGui& gui, Inventory &inv, VoxelManager& vx) {
        removeWidgets(gui);

        UIState::currentState = UIState::nostate;
        UIState::currentState->load(gui, inv, vx);
    }

    static void buildCallback(FinalImageUI& imageUI, std::string& savedReturnPath) {
        savedReturnPath = imageUI.createImageReturnPath(FileGlobal::mouldIndex);
    }

    static void savebuttonCallback(tgui::BackendGui& gui, std::string &path, Inventory &inv, VoxelManager&vx) {
        if(path.empty()) return;

        sf::Image img; 
        img.loadFromFile(path);
        
        inv.addItem(vx, std::make_shared<PlaceItem>(vx, path, path));
        removeWidgets(gui);

        vx.removeMaterialsOfImage(img);

        UIState::currentState = UIState::nostate;
        UIState::currentState->load(gui, inv, vx);
    }
};