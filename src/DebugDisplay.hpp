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

            std::string ECC = "\tElectric comp count   = " + std::to_string(debug_globals::electricity_component_count) + '\n';
            std::string ESC = "\tElectric source count = " + std::to_string(debug_globals::electricity_source_count) + '\n';
            std::string PC  = "\tParticle  count       = " + std::to_string(debug_globals::particle_count) + '\n';
            std::string DT  = "\tFrame time            = " + std::to_string(debug_globals::frame_time) + '\n';

            std::string PPOS= "\tPlayer position       = p(" + std::to_string(debug_globals::player_position.x) + ", " + std::to_string(debug_globals::player_position.y) + ")\n";


            tgui::String final_text;

            final_text += "Simulation info : ";

            final_text += ECC.c_str();
            final_text += ESC.c_str();
            final_text += PC.c_str();
            final_text += DT.c_str();

            final_text += "Player info : ";
            final_text += PPOS;



            debugLabel->setText(final_text);


        }
    private:

        tgui::Label::Ptr debugLabel = nullptr; 
};