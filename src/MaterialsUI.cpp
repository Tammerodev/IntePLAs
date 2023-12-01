#include "MaterialsUI.hpp"

void MaterialsUI::load(tgui::BackendGui &gui) {
    tgui::Theme theme = tgui::Theme("res/themes/nanogui.style");

    canvas = tgui::CanvasSFML::create(tgui::Layout2d(500, 150));
    canvas->setPosition(tgui::Layout2d(500, 0));
    
    canvas->setWidgetName("DEL_");

    addBars("inv");

    gui.add(canvas);

}

void MaterialsUI::render(sf::RenderTarget &target) {
    canvas->clear(UISettings::getColorRGBA());

    for(auto &bar : barChart) {
        canvas->draw(bar);
    }

    canvas->display();
}

void MaterialsUI::update(VoxelManager &vx_manager)
{

    barChart.at(0).setSize(sf::Vector2f(barHeight, materials.carbon));
    barChart.at(1).setSize(sf::Vector2f(barHeight, materials.lithium));
    barChart.at(2).setSize(sf::Vector2f(barHeight, materials.magnesium));
    barChart.at(3).setSize(sf::Vector2f(barHeight, materials.sodium));
    barChart.at(4).setSize(sf::Vector2f(barHeight, materials.aluminium));
    barChart.at(5).setSize(sf::Vector2f(barHeight, materials.silicon));
    barChart.at(6).setSize(sf::Vector2f(barHeight, materials.copper));
    barChart.at(7).setSize(sf::Vector2f(barHeight, materials.titanium));
    barChart.at(8).setSize(sf::Vector2f(barHeight, materials.lead));

    for(auto &bar : barChart) {
        bar.setSize(sf::Vector2f(bar.getSize().x, (bar.getSize().y + 50) / 10));
    }

    MaterialPack& received_mat = vx_manager.getReceivedMaterials();
    addPack(received_mat);
    received_mat = MaterialPack();
}
