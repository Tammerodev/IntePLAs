#pragma once
#include "../BarUI.hpp"
#include "../Player/Player.hpp"
#include <SFML/Graphics.hpp>

#include "../Palettes/PaletteUI.hpp"

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

            sf::Color radiationColor = sf::Color(255, 255, 255, 255);

            int b = 255 - PlayerGlobal::still_radioation / 5;
            int r = 255 - PlayerGlobal::still_radioation / 15.0;


            radiationColor.b = std::clamp(b, 0, 255);
            radiationColor.r = std::clamp(r, 0, 255);

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