#pragma once
#include "ChunkIndexerVoxelContainer.hpp"
#include <thread>
#include "common.hpp"

struct RadioactiveSource {
    sf::Vector2i position;
    int strength;
};



namespace RadiationSearch {

    inline std::list<RadioactiveSource> overpowering_sources;
    inline std::thread thread;

    inline float radiation = 0.f;

    static void s_search(ChunkIndexer& world, sf::Vector2f* p_pos) {
        while(true) {
            const int search_radius = 100;
            radiation = 0.f;
            
            for(int x = 0; x < search_radius; x++) {
                for(int y = 0; y < search_radius; y++) {
                    sf::Vector2i pos = sf::Vector2i(*p_pos) + sf::Vector2i(x, y) - sf::Vector2i(search_radius / 2, search_radius / 2);
                    const float distance = math::distance(*p_pos, sf::Vector2f(pos));

                    const float voxel_rad = elm::getInfoFromType(world.boundGetVoxelAt(pos.x, pos.y).value).radiation;
                    radiation += (voxel_rad / std::clamp(search_radius - distance, 1.f, (float)search_radius));
                }
            }
        }
    }

    inline void update() {
        PlayerGlobal::radiation_received = radiation;
    }

    static void launch(ChunkIndexer& world, sf::Vector2f* pos) {
        thread = std::thread(s_search, std::ref(world), pos);
    }
};