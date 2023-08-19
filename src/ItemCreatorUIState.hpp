#pragma once
#include "UIState.hpp"
#include "Panel.hpp"
#include "Button.hpp"
#include "cmath"
#include "math.hpp"
#include "Elements.hpp"
#include "PlayerUI.hpp"

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/CheckBox.hpp>
#include <TGUI/Widgets/Panel.hpp>


class ItemCreator : public UIState {
public:

    ItemCreator() {
        editImg.create(sizeX, sizeY, sf::Color(0, 0, 0, 0));
        editTex.create(sizeX, sizeY);

        for(int y = 0; y < sizeY; y++) {
            for(int x = 0; x < sizeX; x++) {   
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(scaling, scaling));
                rect.setPosition(x * scaling,y * scaling);
                rect.setFillColor(sf::Color(0,0,0,0));
                rect.setOutlineColor(sf::Color::Black);
                rect.setOutlineThickness(-1);
                rects.push_back(rect);
            }
        }

        preview.setFillColor(sf::Color(50,20,30,120));
        preview.setOutlineColor(sf::Color(100,100,100,50));
        preview.setOutlineThickness(-1.5);
        preview.setSize(sf::Vector2f(scaling, scaling));
    }

    bool load(tgui::BackendGui& gui, Inventory& inv, VoxelManager& vx) {
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
            button_exit->onClick(exitbuttonCallback, std::ref(gui), std::ref(inv), std::ref(vx));

            auto button_save = tgui::Button::create("save");
            button_save->setRenderer(theme.getRenderer("Button"));
            button_save->onClick(savebuttonCallback, std::ref(gui), std::ref(editImg), std::ref(inv), std::ref(vx));
            button_save->setSize(tgui::Layout2d(48 * 4,16 * 4));
            button_save->setPosition(tgui::Layout2d(200,270));

            auto label_sx = tgui::Label::create("x" + std::to_string(sizeX)); 
            label_sx->setPosition(tgui::Layout2d(200, 350));
            label_sx->setRenderer(theme.getRenderer("Label"));

            auto label_sy = tgui::Label::create("y" + std::to_string(sizeY)); 
            label_sy->setPosition(tgui::Layout2d(200, 450));
            label_sy->setRenderer(theme.getRenderer("Label"));

            std::vector<tgui::Button::Ptr> element_buttons;
            element_buttons.push_back(tgui::Button::create("  "));
            element_buttons.push_back(tgui::Button::create("C "));
            element_buttons.push_back(tgui::Button::create("Li"));
            element_buttons.push_back(tgui::Button::create("Mg"));
            element_buttons.push_back(tgui::Button::create("Na"));
            element_buttons.push_back(tgui::Button::create("Al"));
            element_buttons.push_back(tgui::Button::create("Si"));
            element_buttons.push_back(tgui::Button::create("Cu"));
            element_buttons.push_back(tgui::Button::create("Ti"));
            element_buttons.push_back(tgui::Button::create("Pb"));

            // Configure button
            button_exit->setSize(tgui::Layout2d(48 * 4,16 * 4));
            button_exit->setPosition(tgui::Layout2d(200,200));

            canvasSFML = tgui::CanvasSFML::create({sizeX * scaling, sizeY * scaling});
            canvasSFML->setPosition(325, 360);

            gui.add(panel);
            gui.add(button_exit);
            gui.add(button_save);
            gui.add(label_title);
            gui.add(label_sx);
            gui.add(label_sy);
            gui.add(canvasSFML);

            int index = 0;

            for(auto &el_button : element_buttons) {
                index++;

                el_button->setPosition(tgui::Layout2d((450 + 1500) / 2, 200 + index * 32));
                el_button->onClick(element_buttonCallback, std::ref(el_button->getText()), std::ref(selectedColor));
                el_button->setRenderer(theme.getRenderer("Button"));
                gui.add(el_button);
            }


        } catch(std::exception &ex) {
            prnerr("Error at ItemCreator UI state. Could not load gui", ex.what());
        }
        return true;
    }

    void update(const sf::Vector2f &mousepos) {
        editTex.update(editImg);
        editSpr.setTexture(editTex);

        editSpr.setPosition(0,0);
        editSpr.setScale(scaling, scaling);

        position = mousepos;

        isDrawing = math::distance(mousepos, canvasSFML->getPosition()) < ((sizeX * scaling) + (sizeY * scaling));

        snapped_pixpos = sf::Vector2i(relativeMousePosition);
        snapped_pixpos.x /= scaling;
        snapped_pixpos.y /= scaling;

        if(snapped_pixpos.x < 0) snapped_pixpos.x = 0;
        if(snapped_pixpos.y < 0) snapped_pixpos.y = 0;

        if(snapped_pixpos.x > sizeX) snapped_pixpos.x = sizeX;
        if(snapped_pixpos.y > sizeY) snapped_pixpos.y = sizeY;

        preview.setPosition(snapped_pixpos.x * scaling, snapped_pixpos.y * scaling);

        if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && isDrawing) {
            editImg.setPixel(snapped_pixpos.x, snapped_pixpos.y, selectedColor);
        }
    }

    void input(sf::Event &e) {  
        
    }

    void draw(sf::RenderTarget& target, tgui::BackendGui& gui) {        
        canvasSFML->clear(sf::Color(0, 0, 0, 0));
        canvasSFML->draw(editSpr);
        canvasSFML->draw(preview);

        for(const auto &rect : rects) {
            canvasSFML->draw(rect);
        }

        canvasSFML->display();

        // TODO : Clean up. not pretty but it works

        sf::Vector2f spritePosition = editSpr.getPosition();
        sf::Vector2f localMousePosition = target.mapPixelToCoords(sf::Vector2i(position));

        relativeMousePosition = localMousePosition - spritePosition;
        relativeMousePosition.x -= 330;
        relativeMousePosition.y -= 330;

    }

    void statexit() {

    }

private:

    bool isDrawing = true;

    sf::Color selectedColor = elm::Carbon;

    sf::Vector2f position;
    sf::Vector2f relativeMousePosition;
    sf::Vector2i snapped_pixpos;

    sf::Image editImg;
    sf::RectangleShape preview;
    sf::Texture editTex;
    sf::Sprite editSpr;
    std::vector<sf::RectangleShape> rects;

    tgui::CanvasSFML::Ptr canvasSFML = nullptr;


    uint16_t sizeX = 16;
    uint16_t sizeY = 16;
    float scaling = 20.0f;


    static void exitbuttonCallback(tgui::BackendGui& gui, Inventory &inv, VoxelManager& vx) {
        gui.removeAllWidgets();

        UIState::currentState = UIState::nostate;
        UIState::currentState->load(gui, inv, vx);
    }

    static void savebuttonCallback(tgui::BackendGui& gui, sf::Image edImg, Inventory &inv, VoxelManager&vx) {
        // TODO better name
        std::string saveName = "temp/" + std::to_string(time(0)) + ".png";
        edImg.saveToFile(saveName);
        inv.addItem(vx, std::make_shared<PlaceItem>(vx, saveName, saveName));
        gui.removeAllWidgets();

        UIState::currentState = UIState::nostate;
        UIState::currentState->load(gui, inv, vx);
    }

    static void element_buttonCallback(const tgui::String& name, sf::Color &selColor) {
        if(name == "  ") selColor = sf::Color(0, 0, 0, 0);
        else if(name == "C ") selColor = elm::Carbon;
        else if(name == "Li") selColor = elm::Lithium;
        else if(name == "Mg") selColor = elm::Magnesium;
        else if(name == "Na") selColor = elm::Sodium;
        else if(name == "Al") selColor = elm::Aluminium;
        else if(name == "Si") selColor = elm::Silicon;
        else if(name == "Cu") selColor = elm::Copper;
        else if(name == "Ti") selColor = elm::Titanium;
        else if(name == "Pb") selColor = elm::Lead;
    }

};