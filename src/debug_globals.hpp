#pragma once
#include <SFML/System/Vector2.hpp>

namespace debug_globals {
    inline int particle_count = 0;
    inline int mob_count = 0;

    inline int electricity_component_count = 0;
    inline int electricity_source_count = 0;


    inline float frame_time = 0.f;

    inline int loaded_chunks = 0;

    inline sf::Vector2f player_position {0.f, 0.f};


    inline bool inDebugDisplayState = true;
    inline bool photoMode = false;

}

namespace load_state {

    enum LoadState {
        Undefined,

        Pre_initialization,

        Search_devices,

        Initializing_map,
        Creating_map,
        Loading_map,

        Generating_vegetation,
        Unloding_chunks,

        Loading_sprites,
        Loading_effects,
        Loading_player,
        Loading_sounds,

        Loading_ui,
        Compiling_shaders,
        
        Loading_mobs,
        Spawning_mobs,
        Finishing,
    };

    inline const char* getNameOfLoadState(LoadState state) {
        switch (state)
        {

        case Undefined: return "Undefined";
        case Pre_initialization: return "Pre-initilazation";
        case Search_devices: return "Searching for input devices";
        case Initializing_map: return "Initializing map";
        case Creating_map: return "Creating map";
        case Loading_map: return "Loading map";
        case Generating_vegetation: return "Generating vegetation";
        case Unloding_chunks: return "Unloading chunks";
        case Loading_sprites: return "Loading sprites";
        case Loading_effects: return "Loading effects";
        case Loading_player: return "Loading player";
        case Loading_sounds: return "Loading sounds";
        case Loading_ui: return "Loading user interface";
        case Compiling_shaders: return "Compiling GLSL shaders";
        case Loading_mobs: return "Loading entities";
        case Spawning_mobs: return "Spawning entities";
        case Finishing: return "Finishing... (Actually I dont know what its doing)";

        
        default:
            return "";
        }
    } 

    inline LoadState loadState = LoadState::Undefined;


    inline void setState(const LoadState state) {
        load_state::loadState = state;
    }
};