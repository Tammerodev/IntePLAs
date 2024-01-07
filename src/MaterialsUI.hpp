#pragma once
#include <SFML/Graphics.hpp>
#include "VoxelManager.hpp"
#include "MaterialBar.hpp"
#include <iostream>
#include <vector>

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/CheckBox.hpp>
#include <TGUI/Widgets/Panel.hpp>

#include "Settings.hpp"


class MaterialsUI {
    public:
        void load(tgui::BackendGui& gui);
        
        void render(sf::RenderTarget& target);

        void update(VoxelManager &vx_manager);
    private:
        const float barHeight = 10;
        const float barDistance = 15;

        std::array<MaterialBar, 14> barChart;

        bool modified = false;

        tgui::CanvasSFML::Ptr canvas = nullptr;

    private:

        void addBars(const char *name) {
            int i = 2;
            for(auto &bar : barChart) {
                bar = MaterialBar();
                bar.setFillColor(sf::Color());
                bar.setPosition(i * barDistance, 0);
                bar.setSize(sf::Vector2f(10, barHeight));

                bar.setFillColor(elm::getInfoFromType(i).color);

                ++i;
            }
        }
};
