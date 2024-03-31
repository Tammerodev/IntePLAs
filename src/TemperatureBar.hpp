#pragma once
#include "BarUI.hpp"
#include "Player.hpp"
#include <SFML/Graphics.hpp>

#include "Palettes/PaletteUI.hpp"

class TemperatureBar {
    public:
        bool load(const int sizeX, const int sizeY) {
            bar_ui.setIndex(5);

            bar_ui.setColorEmpty(Palette::PaletteUI::Gray);
            bar_ui.setColorFill(Palette::PaletteUI::WineRed);
            bar_ui.setMaxValue(64);

            bar_ui.load(sizeX, sizeY, "res/img/Player/TempBar.png");
            return true;
        }

        void update() {
            const float temp = (PlayerGlobal::getTemparature());
            bar_ui.setValue(temp);
            
            sf::Color barColor = sf::Color(0,40,0,255);

            int red = temp - 36.f;
            int blue = 36.f - temp;

            barColor.r = std::clamp(red, 0, 255);
            barColor.b = std::clamp(blue, 0, 255);

            for(auto &heart : bar_ui.heartSprites) {
                heart.setColor(barColor);
            }

            bar_ui.update();
        }

        void render(sf::RenderTarget& target) {
            bar_ui.render(target);
        } 
    private:
        BarUI bar_ui;
        
};