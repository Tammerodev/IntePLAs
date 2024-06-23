#pragma once
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/CheckBox.hpp>
#include <TGUI/Widgets/Panel.hpp>
#include "BitmapText/BitmapText.hpp"

namespace UI {
    inline sf::Font font;
    inline BitmapTextFont bmp_font;

    inline void loadFont() {
        font.loadFromFile("res/Fonts/VT323.ttf");
        bmp_font.loadFromFile("res/Fonts/bitmap.png", sf::Vector2u(5, 8), 6);
    }
};

class MaterialBar : public sf::RectangleShape {
    public:
        MaterialBar() {
            UI::loadFont();
            bmp_text.setFont(&UI::bmp_font);
            bmp_text_count.setFont(&UI::bmp_font);

        }

        void _setPosition(float x, float y) {
            setPosition(x, y);
            bmp_text.setPosition(sf::Vector2f(x, y + 6));
            bmp_text.setScale(2.0f);

            bmp_text_count.setPosition(sf::Vector2f(x, y + 24));
            bmp_text_count.setScale(2.0f);
        }

        void render(tgui::CanvasSFML::Ptr canvas) {
            canvas->draw(*this);
            
            if(displayText) {
                displayText = false;

                setOutlineColor(sf::Color(255, 255, 255) - getFillColor());
                setOutlineThickness(-1.0);
            } else {
                setOutlineThickness(0.0);
            }
        }

        void showText() {
            displayText = true;
        }

        void setAmount(int x, int y, const int value) {
            setSize(sf::Vector2f(x, y));
            bmp_text.setText(elm::getInfoFromType(value).name);
            bmp_text_count.setNumber(y);
        }

    public:
        BitmapText bmp_text;
        BitmapText bmp_text_count;

        bool displayText = false;
};