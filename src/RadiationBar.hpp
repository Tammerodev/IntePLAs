#pragma once
#include "BarUI.hpp"
#include "Player.hpp"
#include <SFML/Graphics.hpp>

#include "Palettes/PaletteUI.hpp"

class RadiationBar {
    public:
        bool load(const int sizeX, const int sizeY) {
            bar_ui.setIndex(4);

            bar_ui.setColorEmpty(Palette::PaletteUI::Gray);
            bar_ui.setColorFill(Palette::PaletteUI::Yellow);
            bar_ui.setMaxValue(PlayerGlobal::max_health);
            
            bar_ui.load(sizeX, sizeY);

            return true;
        }

        void update() {
            bar_ui.setValue(PlayerGlobal::still_radioation / 4);

            int col = std::clamp(PlayerGlobal::still_radioation, 0, 20);
            int col_a = std::clamp(PlayerGlobal::still_radioation, 0, 255);


            bar_ui.setColorEmpty(sf::Color(col, col, col, col_a));

            bar_ui.update();
        }

        void render(sf::RenderTarget& target) {
            bar_ui.render(target);
        } 
    private:
        BarUI bar_ui;
};