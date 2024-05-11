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

        std::map<unsigned char, MaterialBar> barChart;

        bool modified = false;

        tgui::CanvasSFML::Ptr canvas = nullptr;

    private:

        void addBar(int id) {
            //barChart[id] = MaterialBar();
            //barChart[id].();

        }
};
