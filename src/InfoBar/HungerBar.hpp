#pragma once
#include "../BarUI.hpp"
#include "../Player/Player.hpp"
#include <SFML/Graphics.hpp>

#include "../Palettes/PaletteUI.hpp"

class HungerBar {
    public:
        bool load(const int sizeX, const int sizeY) {
            bar_ui.setIndex(5);

            bar_ui.setColorEmpty(Palette::PaletteUI::Gray);
            bar_ui.setColorFill(Palette::PaletteUI::WineRed);
            bar_ui.setMaxValue(PlayerGlobal::max_hunger);

            bar_ui.load(sizeX, sizeY, "res/img/Player/hunger.png");
            return true;
        }

        void update() {
            bar_ui.setValue(PlayerGlobal::hunger);

            bar_ui.update();
        }

        void render(sf::RenderTarget& target) {
            bar_ui.render(target);
        } 
    private:
        BarUI bar_ui;
        
};