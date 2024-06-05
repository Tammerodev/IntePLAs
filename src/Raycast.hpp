#pragma once
#include <SFML/Graphics.hpp>
#include "Chunk.hpp"
#include "ChunkIndexerVoxelContainer.hpp"
#include <list>
#include "ParticleSimulation.hpp"
#include "Particle/PickableDebris.hpp"
#include "Elements/BurnedMaterial.hpp"

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

        ParticleSimulation* particle_simulation;

        uint64_t world_sx;
        uint64_t world_sy;

        sf::Vector2f* velocity = nullptr;

        int propability_of_material = 0;
        int temp = 0;

        bool isLaser = false;

        bool turnToAsh = false;
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

            if(info.world != nullptr) {
                info.world->boundVector(pixelPosition);
            }

            if(i < info.intensity) {
                if(info.world != nullptr) {
                    const sf::Color originalColor = info.world->getImagePixelAt(pixelPosition.x, pixelPosition.y);

                    info.world->boundVector(pixelPosition);

                    if(info.world->getVoxelAt(pixelPosition.x, pixelPosition.y).value == VoxelValues::MIRROR) {
                        const sf::Vector2i originalStart = info.start;
                        info.end = pixelPosition;
                        info.start = info.end;
                        info.end.y = originalStart.y + 100;

                        delta = info.end - info.start;
                        length = static_cast<int>(std::sqrt(delta.x * delta.x + delta.y * delta.y));

                    }

                    info.world->boundGetChunkAt(info.world->getChunkFromPos(pixelPosition.x, pixelPosition.y).x, info.world->getChunkFromPos(pixelPosition.x, pixelPosition.y).y).needs_update = true;

                    if(force) { 
                        info.world->damageVoxelAt(pixelPosition.x, pixelPosition.y);
                    }

                    if(info.velocity != nullptr) {
                        *info.velocity += (sf::Vector2f(delta.x, delta.y) * t) / 100.0f; 
                    }
                    
                    info.world->heatVoxelAt(pixelPosition.x, pixelPosition.y, info.temp);

                    if(info.world->getVoxelAt(pixelPosition.x, pixelPosition.y).value != 0) {
                        if(info.turnToAsh && math::randProp(10) && math::distance(sf::Vector2f(pixelPosition), sf::Vector2f(info.start)) > info.intensity - 10) {
                            info.world->clearVoxelAt(pixelPosition.x, pixelPosition.y);
                            info.world->getChunkAt(info.world->getChunkFromPos(pixelPosition.x, pixelPosition.y)).elements.emplace_back(
                                std::make_shared<BurnedMaterial>(pixelPosition.x, pixelPosition.y)
                            );
                        } else if(info.voxelsInNeedOfUpdate != nullptr) {
                            info.voxelsInNeedOfUpdate->push_back(pixelPosition);
                        }
                            
                        power--;
                        if(power <= 0) break;
                    }  else if(originalColor.a != 0) {
                        if(math::randIntInRange(0, 100) < info.propability_of_material) {
                            /*info.particle_simulation->addParticle(
                                std::make_shared<PickableDebris>(
                                    sf::Vector2f(pixelPosition), math::subVector(sf::Vector2f(pixelPosition), sf::Vector2f(info.start)) / 10.f,
                                    originalColor,
                                    info.world->getVoxelAt(pixelPosition.x, pixelPosition.y).value,
                                    info.world
                                    ));*/
                        }
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