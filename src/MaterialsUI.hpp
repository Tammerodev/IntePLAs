#pragma once
#include <SFML/Graphics.hpp>
#include "VoxelManager.hpp"
#include <iostream>
#include <vector>

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/CheckBox.hpp>
#include <TGUI/Widgets/Panel.hpp>

class MaterialsUI {
public:
    void load(tgui::BackendGui& gui);
    
    void render(sf::RenderTarget& target) {
        for(auto &bar : barChart) {
            target.draw(bar);
        }
    }

    void update(VoxelManager &vx_manager);
private:
    MaterialPack materials;

    const float barHeight = 10;
    const float barDistance = 15;

    std::array<sf::RectangleShape, 9> barChart;

    bool modified = false;
private:

    void addPack(const MaterialPack& matPack) {
        materials.carbon += matPack.carbon;
        materials.lithium += matPack.lithium;
        materials.magnesium += matPack.magnesium;
        materials.sodium+= matPack.sodium;
        materials.aluminium += matPack.aluminium;
        materials.silicon += matPack.silicon;
        materials.copper += matPack.copper;
        materials.titanium += matPack.titanium;
        materials.lead += matPack.lead;

        modified = true;
    }
};
