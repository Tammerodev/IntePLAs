#pragma once
#include <TGUI/TGUI.hpp>

class SelectMouldUI {
    public:
        void load(tgui::BackendGui& gui) {
            panel = tgui::Panel::create(tgui::Layout2d(48 * scaling, 48 * scaling));

            gui.add(panel);

            grid = tgui::Grid::create();
            grid->setPosition(panel->getPosition());
            grid->setSize(panel->getSize());

            const int numRows = 3;
            const int numCols = 3;

            for (int row = 0; row < numRows; ++row) {
                for (int col = 0; col < numCols; ++col) {
                    int left = (row + (col * numCols)) * 16;
                    mouldTextures.load("res/img/Item/moulds.png", tgui::UIntRect(left, 0, 16, 16));

                    auto picture = tgui::Picture::create(mouldTextures);
                    picture->setSize(16 * scaling, 16 * scaling);
                    picture->setWidgetName(std::to_string(left));

                    grid->addWidget(picture, col, row);
                }
            }

            gui.add(grid);

            panel->setVisible(false);
            grid->setVisible(false);
        }  

        void show(const tgui::Layout2d &position) {
            panel->setPosition(position);
            grid->setPosition(position);
            panel->setVisible(true);
            grid->setVisible(true);

            panel->moveToFront();
            grid->moveToFront();
        }

        void hide() {
            panel->setVisible(false);
            grid->setVisible(false);
        }

        const tgui::String update(tgui::BackendGui& gui) {
            auto widgets = grid->getWidgets();

            for(const auto& widget : widgets) {
                // Yep, its bad but it's just 9 iterations... ... ...
                if(widget->isMouseDown()) {
                    return widget->getWidgetName();
                }
            }

            return "";
        }

    private:
        tgui::Panel::Ptr panel = nullptr;
        tgui::Grid::Ptr grid = nullptr;
        tgui::Texture mouldTextures;

        float scaling = 5.f;
};