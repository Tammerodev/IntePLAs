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

    BitmapText* displayText = nullptr;
    BitmapText* displayTextNum = nullptr;

    for(auto &bar : barChart) {
        bar.second.move(canvas->getPosition());

        if(bar.second.getGlobalBounds().contains(Controls::windowCursorPos) && Controls::windowCursorPos.y < canvas->getSize().y) {
            MaterialsUIGlobal::focusedOnBar = bar.first;
            bar.second.showText();
            displayText = &bar.second.bmp_text;
            displayTextNum = &bar.second.bmp_text_count;
        }

        bar.second.move(-canvas->getPosition());

        bar.second.render(canvas);

        index++;
    }

    if(displayText != nullptr && displayTextNum != nullptr) {
        displayText->render(canvas);
        displayTextNum->render(canvas);
    }

    canvas->display();
}

void MaterialsUI::update(VoxelManager &vx_manager)
{
    updateBars(vx_manager.getChunkIndexer().materialpack);
}
