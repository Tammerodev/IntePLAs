#pragma once
#include <TGUI/AllWidgets.hpp>
#include "../../WidgetManager.hpp"

class HeistInfoGui {
    public:
        void load(tgui::BackendGui& gui) {
            boxes_collected = tgui::Label::create();
            boxes_collected->setWidgetName(DONT_REMOVE_WIDGET);
            gui.add(boxes_collected);
        }

        void update(const int collected) {
            boxes_collected->setSize(150, 75);
            boxes_collected->setPosition(Display::window_width - 160, Display::window_height - 85 );

            boxes_collected->setText("Boxes collected : " + collected);
        }
    private:
        tgui::Label::Ptr boxes_collected = nullptr;
};