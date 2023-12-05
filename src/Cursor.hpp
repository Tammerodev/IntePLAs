#pragma once
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Widget.hpp>

#include "Controls.hpp"

class Cursor : private sf::Sprite {
public:
    bool load(tgui::BackendGui &gui, sf::Window &window) {
        bool res = tx.loadFromFile("res/img/UI/cursor.png");

        if (cursor.loadFromPixels(tx.copyToImage().getPixelsPtr(), { 32, 32 }, { 16, 16 })) {
            window.setMouseCursor(cursor);
        } else {
            res = false;
        }
        return res;
    }
private:
    sf::Texture tx;   
    sf::Cursor cursor; 
};