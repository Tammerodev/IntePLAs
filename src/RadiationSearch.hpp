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


    static void searchVoxel(int x, int y, int search_radius, ChunkIndexer& world) {
        const float voxel_rad = elm::getInfoFromType(world.boundGetVoxelAt(x, y).value).radiation;
        radiation += voxel_rad / 35.f;
    }

    static void s_search(ChunkIndexer& world, sf::Vector2f* p_pos) {
        while(true) {
            const int search_radius = 50;
            radiation = 0.f;

            /* We need to perform a rectangled "spiral search" from the start points
               Due to multithreading*/
            sf::Vector2i pos = sf::Vector2i(*p_pos) + sf::Vector2i(search_radius / 2, search_radius / 2);
            int lineLength = 1;

            int max_distance = search_radius;

            for (int distance = 0; distance <= max_distance; ++distance) {
                for (int y = -distance; y <= distance; ++y) {
                    for (int x = -distance; x <= distance; ++x) {
                        if (abs(x) == distance || abs(y) == distance) {
                            searchVoxel(x + pos.x, y + pos.y, search_radius, world);
                        }
                    }
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