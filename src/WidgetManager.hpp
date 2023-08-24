#pragma once 
#include <TGUI/TGUI.hpp>


static void removeWidgets(tgui::BackendGui& gui) {
    const auto widgets = gui.getWidgets();

    for(const auto &widget : widgets) {
        if(widget != nullptr && widget->getWidgetName() != "Cursor")
            gui.remove(widget);
    }
}
