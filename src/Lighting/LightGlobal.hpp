#pragma once
#include <map>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>

#include "../ChunkIndexerVoxelContainer.hpp"
#include "../ElectricityGlobal.hpp"

namespace LightGlobal {

    struct LightElement {
        LightElement(const int intens, const sf::Color &col) {
            intensity = intens;
            color = col;
        }

        int intensity = 0;
        sf::Color color = sf::Color::White;

    };

    // MAP:         Position    ,light intensity
    inline std::map<ComparableVector2i, LightElement> source_positions {};

    inline void add_source(const sf::Vector2i pos, const LightElement &light_data) {
        ComparableVector2i pos_c;
        pos_c.x = pos.x;
        pos_c.y = pos.y;

        source_positions.insert({pos_c, light_data});
    }

    inline void remove_source(const sf::Vector2i pos) {
        ComparableVector2i pos_c;
        pos_c.x = pos.x;
        pos_c.y = pos.y;

        source_positions.erase(pos_c);
    }

    static void check_sources(ChunkIndexer &world) {
        for (auto it = source_positions.cbegin(), next_it = it; it != source_positions.cend(); it = next_it)
        {
            ++next_it;
            if (world.boundGetVoxelAt(it->first.x, it->first.y).value == 0)
            {
                source_positions.erase(it);
            }
        }

    }
}