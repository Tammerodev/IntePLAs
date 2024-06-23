#include "MaterialsUI.hpp"

void MaterialsUI::load(tgui::BackendGui &gui) {
    tgui::Theme theme = tgui::Theme("res/themes/nanogui.style");
    
    UI::loadFont();
    canvas = tgui::CanvasSFML::create(tgui::Layout2d(500, 150));
    canvas->setPosition(tgui::Layout2d(500, 0));
    
    canvas->setWidgetName("DEL_");

    gui.add(canvas);

}

void MaterialsUI::render(sf::RenderTarget &target) {
    canvas->clear(UISettings::getColorRGBA());

    int index = 0;

    for(auto &bar : barChart) {
        canvas->draw(bar.second);

        bar.second.move(canvas->getPosition());

        if(bar.second.getGlobalBounds().contains(Controls::windowCursorPos) && Controls::windowCursorPos.y < canvas->getSize().y) {
            MaterialsUIGlobal::focusedOnBar = bar.first;
        }

        bar.second.move(-canvas->getPosition());

        index++;
    }

    canvas->display();
}

void MaterialsUI::update(VoxelManager &vx_manager)
{
    updateBars(vx_manager.getChunkIndexer().materialpack);
}
