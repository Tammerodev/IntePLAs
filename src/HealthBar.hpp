#pragma once
#include "BarUI.hpp"
#include "Player.hpp"
#include <SFML/Graphics.hpp>

#include "Palettes/PaletteUI.hpp"

class HealthBar {
    public:
        bool load(const int sizeX, const int sizeY) {
            bar_ui.setIndex(2);

            bar_ui.setColorEmpty(Palette::PaletteUI::Gray);
            bar_ui.setColorFill(Palette::PaletteUI::WineRed);
            bar_ui.setMaxValue(PlayerGlobal::max_health);

            bar_ui.load(sizeX, sizeY);
            return true;
        }

        void update() {
            bar_ui.setValue(PlayerGlobal::health);

            sf::Color radiationColor = {255, 255, 255, 255};
            radiationColor.r -= PlayerGlobal::still_radioation / 500;
            prndd(PlayerGlobal::still_radioation);

            for(auto &heart : bar_ui.heartSprites) {
                heart.setColor(radiationColor);
            }

            bar_ui.update();
        }

        void render(sf::RenderTarget& target) {
            bar_ui.render(target);
        } 
    private:
        BarUI bar_ui;
        
};