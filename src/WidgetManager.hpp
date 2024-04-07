#pragma once 
#include <TGUI/TGUI.hpp>
#define DONT_REMOVE_WIDGET "DEL_"

static void removeWidgets(tgui::BackendGui& gui) {
    const auto widgets = gui.getWidgets();

    for(const auto &widget : widgets) {
        if(widget != nullptr && widget->getWidgetName() != DONT_REMOVE_WIDGET)
            gui.remove(widget);
    }
}
