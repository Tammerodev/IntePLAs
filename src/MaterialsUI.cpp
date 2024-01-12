#include "MaterialsUI.hpp"

void MaterialsUI::load(tgui::BackendGui &gui) {
    tgui::Theme theme = tgui::Theme("res/themes/nanogui.style");
    
    UI::loadFont();
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

    barChart.at(0).setAmount(barHeight, vx_manager.getChunkIndexer().materialpack.carbon);
    barChart.at(1).setAmount(barHeight, vx_manager.getChunkIndexer().materialpack.lithium);
    barChart.at(2).setAmount(barHeight, vx_manager.getChunkIndexer().materialpack.magnesium);
    barChart.at(3).setAmount(barHeight, vx_manager.getChunkIndexer().materialpack.sodium);
    barChart.at(4).setAmount(barHeight, vx_manager.getChunkIndexer().materialpack.aluminium);
    barChart.at(5).setAmount(barHeight, vx_manager.getChunkIndexer().materialpack.silicon);
    barChart.at(6).setAmount(barHeight, vx_manager.getChunkIndexer().materialpack.copper);
    barChart.at(7).setAmount(barHeight, vx_manager.getChunkIndexer().materialpack.titanium);
    barChart.at(8).setAmount(barHeight, vx_manager.getChunkIndexer().materialpack.lead);
}
