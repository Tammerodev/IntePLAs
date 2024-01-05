#pragma once
#include <SFML/System/Vector2.hpp>

namespace debug_globals {
    inline int particle_count = 0;
    inline int electricity_component_count = 0;
    inline int electricity_source_count = 0;
    inline float frame_time = 0.f;

    inline sf::Vector2f player_position {0.f, 0.f};
}