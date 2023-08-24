#pragma once
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Widget.hpp>

#include "Entity/Player/Controls.hpp"

class Cursor : private sf::Sprite {
public:
    bool load(tgui::BackendGui &gui) {
        bool res = tx.loadFromFile("res/themes/cross.png");
        setTexture(tx);

        setOrigin(getGlobalBounds().width / 2, getGlobalBounds().height / 2);

        canvas = tgui::CanvasSFML::create();
        canvas->setSize(tgui::Layout2d(1980, 1080));
        canvas->setWidgetName("Cursor");

        // If we dont do this, the canvas will grab the focus and disable all other UI elements
        canvas->setFocusable(false);
        canvas->setEnabled(false);
        canvas->setFocused(false);
        //gui.add(canvas);
        return res;
    }

    void draw(sf::RenderTarget &target) {
        canvas->clear(sf::Color::Transparent);
        setPosition(Controls::windowCursorPos);


        canvas->draw(*this);

        canvas->display();
    }
private:
    tgui::CanvasSFML::Ptr canvas = nullptr;
    sf::Texture tx;    
};