#include "MaterialsUI.hpp"

void MaterialsUI::load(tgui::BackendGui &gui) {

    auto panel = tgui::Panel::create();
    tgui::Theme theme = tgui::Theme("res/themes/nanogui.style");

    panel->setSize(tgui::Layout2d(450,400));
    panel->setWidgetName("DEL_");
    panel->setRenderer(theme.getRenderer("Panel"));

    for(int i = 0; i < barChart.size(); i++) {
        barChart.at(i) = sf::RectangleShape();
        barChart.at(i).setFillColor(sf::Color(201, 58, 77, 150));
        barChart.at(i).setPosition(-300, i * barDistance);
        barChart.at(i).setSize(sf::Vector2f(10, barHeight));
    }

    gui.add(panel);

}


void MaterialsUI::update(VoxelManager &vx_manager)
{

    barChart.at(0).setSize(sf::Vector2f(10 + materials.carbon, barHeight));
    barChart.at(1).setSize(sf::Vector2f(10 + materials.lithium, barHeight));
    barChart.at(2).setSize(sf::Vector2f(10 + materials.magnesium, barHeight));
    barChart.at(3).setSize(sf::Vector2f(10 + materials.sodium, barHeight));
    barChart.at(4).setSize(sf::Vector2f(10 + materials.aluminium, barHeight));
    barChart.at(5).setSize(sf::Vector2f(10 + materials.silicon, barHeight));
    barChart.at(6).setSize(sf::Vector2f(10 + materials.copper, barHeight));
    barChart.at(7).setSize(sf::Vector2f(10 + materials.titanium, barHeight));
    barChart.at(8).setSize(sf::Vector2f(10 + materials.lead, barHeight));

    for(auto &bar : barChart) {
        bar.setSize(sf::Vector2f(bar.getSize().x / 10, bar.getSize().y));
    }

    MaterialPack& received_mat = vx_manager.getReceivedMaterials();
    addPack(received_mat);
    received_mat = MaterialPack();
}
