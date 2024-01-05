#pragma once
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/CheckBox.hpp>

#include "debug_globals.hpp"

class DebugDisplay {
    public:
        void load(tgui::BackendGui& gui) {
            debugLabel = tgui::Label::create("");
            debugLabel->setPosition(tgui::Layout2d(250, 50));
            debugLabel->setSize(tgui::Layout2d(500, 300));
            debugLabel->setWidgetName("DEL_");

            gui.add(debugLabel);
        }

        void render(sf::RenderTarget& target) {
            if(debugLabel == nullptr) return;

            std::string ECC = "Electric comp count   = " + std::to_string(debug_globals::electricity_component_count) + '\n';
            std::string ESC = "Electric source count = " + std::to_string(debug_globals::electricity_source_count) + '\n';
            std::string PC  = "Particle  count       = " + std::to_string(debug_globals::particle_count) + '\n';

            tgui::String final_text;
            final_text += ECC.c_str();
            final_text += ESC.c_str();
            final_text += PC.c_str();

            debugLabel->setText(final_text);


        }
    private:

        tgui::Label::Ptr debugLabel = nullptr; 
};