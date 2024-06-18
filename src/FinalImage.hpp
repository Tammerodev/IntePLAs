#pragma once
#include <TGUI/TGUI.hpp>
#include <array>

class ConnectLine {
    public:
        void load(tgui::BackendGui& gui) {
            lineLeft1 = tgui::Label::create();
            lineUp    = tgui::Label::create();
            lineLeft2 = tgui::Label::create();

            gui.add(lineLeft1);
            gui.add(lineUp);
            gui.add(lineLeft2);
        }

        int isConnected() {
            return connected;
        }

        void setConnected(int c) {
            connected = c;
        }

        void connect(tgui::Layout2d startPos, tgui::Layout2d endPos) {
            float x_off = endPos.x.getValue() - startPos.x.getValue();
            float y_off = endPos.y.getValue() - startPos.y.getValue();

            prndd("connect");

            if(connected == -1) connected = 0;

            lineLeft1->setVisible(true);
            lineUp->setVisible(true);
            lineLeft2->setVisible(true);

            lineUp->setSize(5, y_off);
            lineLeft1->setSize(x_off / 2, 5);
            lineLeft2->setSize(x_off / 2, 5);

            lineLeft1->setPosition(startPos);
            lineUp->setPosition(startPos + tgui::Layout2d(x_off / 2, -endPos.y));
            lineLeft2->setPosition(lineUp->getPosition());

        }
    private:
        tgui::Label::Ptr lineLeft1 = nullptr;
        tgui::Label::Ptr lineUp    = nullptr;
        tgui::Label::Ptr lineLeft2 = nullptr;

        const float thickness = 5;
        int connected = -1;
};

class SelectColor {
    public:
        void load(tgui::BackendGui& gui, const tgui::Layout2d position) {
            panel = tgui::Panel::create();
            panel->getRenderer()->setProperty("BackgroundColor", tgui::ObjectConverter(tgui::Color::Red));
            panel->setPosition(position.x, position.y);
            panel->setSize(25, 25);


            connectLine.load(gui);
            gui.add(panel);
        }  

        void update() {
            if(panel->isMouseDown() || connectLine.isConnected() == 0) {
                connectLine.connect(panel->getPosition(), tgui::Layout2d(Controls::windowCursorPos));

                if(MaterialsUIGlobal::focusedOnBar != -1) {
                    connectLine.setConnected(1);
                    MaterialsUIGlobal::focusedOnBar = -1;
                }
            }
        }
    private:
        tgui::Panel::Ptr panel = nullptr;
        ConnectLine connectLine;
};

class FinalImageUI {
    public:
        void load(tgui::BackendGui& gui, const tgui::Layout2d pos) {
            tgui::Layout2d position = tgui::Layout2d("50%", "25%");

            int index = 0;

            for(auto &s : selectColors) {
                s.load(gui, pos + tgui::Layout2d(0, index * 35));

                ++index;
            }
        }  

        const void update() {
            for(auto &s : selectColors) {
                s.update();
            }
        }


    private:
        std::array<SelectColor, 3> selectColors;
};