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

namespace MaterialsUIGlobal {
    inline int focusedOnBar = -1;

}

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

        int id = 0;

        void addBar(int c, int addAmount) {
            if(barChart.count(c) == 0) {
                id++;
                barChart.insert({c, MaterialBar()});
                barChart[c]._setPosition(id * barDistance, 0);
            }

            barChart[c].setFillColor(elm::getInfoFromType(c).color);
            barChart[c].setAmount(barDistance, 5 + addAmount);
        }


        void updateBars(MaterialPack& pack) {
            for(auto m : pack.materials) {
                if(m.second.amount != 0)
                    addBar(m.first, m.second.amount);
            }
        }
};
