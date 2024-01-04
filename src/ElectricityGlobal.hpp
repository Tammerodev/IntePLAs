#pragma once
#include <map>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>

#include "ChunkIndexerVoxelContainer.hpp"

class ComparableVector2i : public sf::Vector2i {
    public:
        // ERROR : no match for ‘operator<’ (operand types are ‘const ComparableVector2i’ and ‘const ComparableVector2i’)
        bool operator<(const ComparableVector2i& other) const {
            return (x < other.x) || ((x == other.x) && (y < other.y));
        }
};

namespace ElectricityGlobal {
    // MAP:         Position    ,voltage
    inline std::map<ComparableVector2i, int> source_positions {};

    inline void add_source(const sf::Vector2i pos, const int voltage) {
        ComparableVector2i pos_c;
        pos_c.x = pos.x;
        pos_c.y = pos.y;

        source_positions.insert({pos_c, voltage});
    }

    static void check_sources(ChunkIndexer world) {
        for(const auto &source : source_positions) {
            // Check if power source generates power anymore
            if(world.boundGetVoxelAt(source.first.x, source.first.y).value == 0) {
                source_positions.erase(source.first);
            }
        }
    }
}