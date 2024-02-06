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
            debugLabel->setPosition(tgui::Layout2d(100, 50));
            debugLabel->setSize(tgui::Layout2d(500, 300));
            debugLabel->setWidgetName("DEL_");

            gui.add(debugLabel);
        }

        void render(sf::RenderTarget& target) {
            debug_globals::inDebugDisplayState = Controls::showDebugInfo();

            if(debugLabel == nullptr) return;


            debugLabel->setVisible(debug_globals::inDebugDisplayState);

            if(!debug_globals::inDebugDisplayState) return;

            std::string MC  = "\tMob count             = " + std::to_string(debug_globals::mob_count) + '\n';
            std::string ECC = "\tElectric comp count   = " + std::to_string(debug_globals::electricity_component_count) + '\n';
            std::string ESC = "\tElectric source count = " + std::to_string(debug_globals::electricity_source_count) + '\n';
            std::string PC  = "\tParticle  count       = " + std::to_string(debug_globals::particle_count) + '\n';
            std::string DT  = "\tFrame time            = " + std::to_string(debug_globals::frame_time) + '\n';

            std::string CL  = "\tChunks loaded         = " + std::to_string(debug_globals::loaded_chunks) + '\n';
            std::string WS  = "\tWorld size (voxels)   = xy(" + std::to_string(worldSize::world_sx) + ", " + std::to_string(worldSize::world_sy) + ")\n";


            std::string PPOS= "\tPlayer position       = p(" + std::to_string(debug_globals::player_position.x) + ", " + std::to_string(debug_globals::player_position.y) + ")\n";


            tgui::String final_text;

            final_text += "Simulation info : ";

            final_text += MC.c_str();
            final_text += ECC.c_str();
            final_text += ESC.c_str();
            final_text += PC.c_str();
            final_text += DT.c_str();

            final_text += "World info : ";
            final_text += CL.c_str();
            final_text += WS.c_str();


            final_text += "Player info : ";
            final_text += PPOS;



            debugLabel->setText(final_text);


        }
    private:

        tgui::Label::Ptr debugLabel = nullptr; 
};