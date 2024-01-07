#pragma once
#include "UIState.hpp"
#include "Panel.hpp"
#include "Button.hpp"
#include "cmath"
#include "math.hpp"
#include "Elements.hpp"
#include "PlayerUI.hpp"
#include "WidgetManager.hpp"

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/CheckBox.hpp>
#include <TGUI/Widgets/Panel.hpp>


class ItemCreator : public UIState {
public:

    ItemCreator() {
        editImg.create(size.x, size.y, sf::Color(0, 0, 0, 0));
        editTex.create(size.x, size.y);

        for(int y = 0; y < size.y; y++) {
            for(int x = 0; x < size.x; x++) {   
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

            sizeXinc = tgui::Button::create("x+");
            sizeXdec = tgui::Button::create("x-");
            sizeYinc = tgui::Button::create("y+");
            sizeYdec = tgui::Button::create("y-");

            sizeXinc->setPosition(tgui::Layout2d(200, 350));
            sizeXdec->setPosition(tgui::Layout2d(230, 350));
            sizeYinc->setPosition(tgui::Layout2d(200, 380));
            sizeYdec->setPosition(tgui::Layout2d(230, 380));

            sizeXinc->onClick(resize, std::ref(sizeXinc->getText()), std::ref(size), std::ref(scaling), std::ref(rects), std::ref(editImg), std::ref(editTex));
            sizeXdec->onClick(resize, std::ref(sizeXdec->getText()), std::ref(size), std::ref(scaling), std::ref(rects), std::ref(editImg), std::ref(editTex));

            sizeYinc->onClick(resize, std::ref(sizeYinc->getText()), std::ref(size), std::ref(scaling), std::ref(rects), std::ref(editImg), std::ref(editTex));
            sizeYdec->onClick(resize, std::ref(sizeYdec->getText()), std::ref(size), std::ref(scaling), std::ref(rects), std::ref(editImg), std::ref(editTex));


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
            element_buttons.push_back(tgui::Button::create("H2O"));
            element_buttons.push_back(tgui::Button::create("C3H5N3O9"));
            element_buttons.push_back(tgui::Button::create("Cl"));
            element_buttons.push_back(tgui::Button::create("U235"));
            element_buttons.push_back(tgui::Button::create("Ra226"));
            element_buttons.push_back(tgui::Button::create("Sand"));
            element_buttons.push_back(tgui::Button::create("Acid"));


            // Configure button
            button_exit->setSize(tgui::Layout2d(48 * 4,16 * 4));
            button_exit->setPosition(tgui::Layout2d(200,200));

            canvasSFML = tgui::CanvasSFML::create(tgui::Layout2d(size.x * scaling, size.y * scaling));
            canvasSFML->setPosition(325, 360);

            gui.add(panel);
            gui.add(button_exit);
            gui.add(button_save);
            gui.add(label_title);

            gui.add(sizeXinc);
            gui.add(sizeXdec);

            gui.add(sizeYinc);
            gui.add(sizeYdec);

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
        sf::Vector2f spritePosition = canvasSFML->getPosition();
        sf::Vector2f localMousePosition = mousepos;

        relativeMousePosition = localMousePosition - spritePosition;


        editTex.update(editImg);
        editSpr.setTexture(editTex);

        editSpr.setPosition(0,0);
        editSpr.setScale(scaling, scaling);

        isDrawing = math::distance(mousepos, canvasSFML->getPosition()) < ((size.x * scaling) + (size.y * scaling));

        snapped_pixpos = sf::Vector2i(relativeMousePosition);
        snapped_pixpos.x /= scaling;
        snapped_pixpos.y /= scaling;

        if(snapped_pixpos.x < 0) snapped_pixpos.x = 0;
        if(snapped_pixpos.y < 0) snapped_pixpos.y = 0;

        if(snapped_pixpos.x > size.x) snapped_pixpos.x = size.x;
        if(snapped_pixpos.y > size.y) snapped_pixpos.y = size.y;

        preview.setPosition(snapped_pixpos.x * scaling, snapped_pixpos.y * scaling);

        if(Controls::useUI() && isDrawing) {
            editImg.setPixel(snapped_pixpos.x, snapped_pixpos.y, selectedColor);
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::K) && isDrawing) {
            for(int y = 0; y < size.y; y++) {
                for(int x = 0; x < size.x; x++) {
                    editImg.setPixel(x, y, selectedColor);
                }
            }
        }

        canvasSFML->setSize(size.x * scaling, size.y * scaling);
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

    }


    void statexit() {

    }

private:

    tgui::Button::Ptr sizeXinc = nullptr;
    tgui::Button::Ptr sizeXdec = nullptr;
    tgui::Button::Ptr sizeYinc = nullptr;
    tgui::Button::Ptr sizeYdec = nullptr;

    tgui::CanvasSFML::Ptr canvasSFML = nullptr;

    bool isDrawing = true;

    sf::Color selectedColor = elm::getInfoFromType(VoxelValues::CARBON).color;

    sf::Vector2f position;
    sf::Vector2f relativeMousePosition;
    sf::Vector2i snapped_pixpos;

    sf::Image editImg;
    sf::RectangleShape preview;
    sf::Texture editTex;
    sf::Sprite editSpr;
    std::list<sf::RectangleShape> rects;

    sf::Vector2i size = sf::Vector2i(16, 16);
    

    int scaling = 20;

    static void exitbuttonCallback(tgui::BackendGui& gui, Inventory &inv, VoxelManager& vx) {
        removeWidgets(gui);

        UIState::currentState = UIState::nostate;
        UIState::currentState->load(gui, inv, vx);
    }

    static void savebuttonCallback(tgui::BackendGui& gui, sf::Image edImg, Inventory &inv, VoxelManager&vx) {
        // TODO better name
        std::string saveName = "temp/" + std::to_string(time(0)) + ".png";
        edImg.saveToFile(saveName);
        inv.addItem(vx, std::make_shared<PlaceItem>(vx, saveName, saveName));
        removeWidgets(gui);

        UIState::currentState = UIState::nostate;
        UIState::currentState->load(gui, inv, vx);
    }

    static void element_buttonCallback(const tgui::String& name, sf::Color &selColor) {
        if(name == "  ") selColor = sf::Color(0, 0, 0, 0);
        else if(name == "C ") selColor = elm::getInfoFromType(VoxelValues::CARBON).color;
        else if(name == "Li") selColor = elm::getInfoFromType(VoxelValues::LITHIUM).color;
        else if(name == "Mg") selColor =elm::getInfoFromType(VoxelValues::MAGNESIUM).color;
        else if(name == "Na") selColor = elm::getInfoFromType(VoxelValues::SODIUM).color;
        else if(name == "Al") selColor = elm::getInfoFromType(VoxelValues::ALUMINIUM).color;
        else if(name == "Si") selColor = elm::getInfoFromType(VoxelValues::SILICON).color;
        else if(name == "Cu") selColor = elm::getInfoFromType(VoxelValues::COPPER).color;
        else if(name == "Ti") selColor = elm::getInfoFromType(VoxelValues::TITANIUM).color;
        else if(name == "Pb") selColor = elm::getInfoFromType(VoxelValues::LEAD).color;
        else if(name == "H2O") selColor = elm::getInfoFromType(VoxelValues::WATER).color;
        else if(name == "C3H5N3O9") selColor = elm::getInfoFromType(VoxelValues::NITROGLYCERIN).color;
        else if(name == "Cl") selColor = elm::getInfoFromType(VoxelValues::CHLORINE).color;
        else if(name == "U235") selColor = elm::getInfoFromType(VoxelValues::URANIUM235).color;
        else if(name == "Ra226") selColor = elm::getInfoFromType(VoxelValues::RADIUM226).color;
        else if(name == "Sand") selColor = elm::getInfoFromType(VoxelValues::SAND).color;
        else if(name == "Acid") selColor = elm::getInfoFromType(VoxelValues::ACID).color;
        
    }

    static void resize(const tgui::String& name, sf::Vector2i &currentSize, int &scaling, std::list<sf::RectangleShape> &rects, sf::Image &img, sf::Texture &tx) {
        if(name == "x+") currentSize.x++;
        if(name == "x-") currentSize.x--;
        if(name == "y+") currentSize.y++;
        if(name == "x-") currentSize.y--;

        if(currentSize.x < 0) currentSize.x = 0;
        if(currentSize.y < 0) currentSize.y = 0;


        rects.clear();

        for(int y = 0; y < currentSize.y; y++) {
            for(int x = 0; x < currentSize.x; x++) {   
                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(scaling, scaling));
                rect.setPosition(x * scaling,y * scaling);
                rect.setFillColor(sf::Color(0,0,0,0));
                rect.setOutlineColor(sf::Color::Black);
                rect.setOutlineThickness(-1);
                rects.push_back(rect);
            }
        }

        img.create(8, 8);
        tx.loadFromImage(img);

        //std::this_thread::sleep_for(std::chrono::seconds(5));
        
    }
};