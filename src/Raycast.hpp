#pragma once
#include <SFML/Graphics.hpp>
#include "Chunk.hpp"
#include "ChunkIndexerVoxelContainer.hpp"
#include <list>
#include "ParticleSimulation.hpp"

namespace Raycast {

    struct RaycastInfo {
        RaycastInfo(ChunkIndexer* indexer) : world(indexer) {

        } 

        sf::Vector2i start {0, 0};
        sf::Vector2i end {0, 0};

        int intensity = 0;
        ChunkIndexer* world = nullptr;
        std::list<sf::Vector2i>* voxelsInNeedOfUpdate = nullptr;

        float longestRay = 0.f;
        sf::Vector2f longestRayVector = {0,0};

        std::list<std::pair<sf::Vector2i, sf::Color>> particles;

        uint64_t world_sx;
        uint64_t world_sy;
        
        MaterialPack* matPack = nullptr;
    };

    static void castRayLine(RaycastInfo &info, bool force = true) {
        sf::Vector2i delta = info.end - info.start;
        int length = static_cast<int>(std::sqrt(delta.x * delta.x + delta.y * delta.y));
        float power = info.intensity / 3;
        power++;

        int i = 0;
        for (; i <= length; i++) {
            float t = static_cast<float>(i) / length;
            sf::Vector2i pixelPosition(
                info.start.x + static_cast<unsigned>(delta.x * t),
                info.start.y + static_cast<unsigned>(delta.y * t)
            );

            if(i < info.intensity) {
                if(info.world != nullptr) {
                    info.world->boundVector(pixelPosition);

                    if(math::randIntInRange(0, 100) < 5) {
                        info.particles.push_back(std::pair(pixelPosition,  info.world->getImagePixelAt(pixelPosition.x, pixelPosition.y))
                        );
                    }

                    if(force) info.world->damageVoxelAt(pixelPosition.x, pixelPosition.y);
                    
                    info.world->heatVoxelAt(pixelPosition.x, pixelPosition.y, 1000);

                    if(info.world->getVoxelAt(pixelPosition.x, pixelPosition.y).value != 0) {
                        if(info.voxelsInNeedOfUpdate != nullptr)
                            info.voxelsInNeedOfUpdate->push_back(pixelPosition);

                        power--;
                        if(power <= 0) break;
                    }
                }
            }
        }

        if(i > info.longestRay) {
            info.longestRay = i;
            info.longestRayVector = math::subVector(sf::Vector2f(info.start), sf::Vector2f(info.end));
        }
    }
}