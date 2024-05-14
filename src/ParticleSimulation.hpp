#pragma once
#include "Particle/Particle.hpp"
#include "Particle/FreeNeutron.hpp"
#include "Particle/DebrisParticle.hpp"
#include "Player/PlayerGlobal.hpp"

#include <list>
#include <memory>

class ParticleSimulation {
    public:
        void addParticle(const std::shared_ptr<Particle> newParticle) {
            particles.push_back(newParticle);
        }

        void update(const float dt, sf::Vector2f playerPosition) {
            playerPosition.x += PlayerGlobal::characterSize.x / 2;
            playerPosition.y += PlayerGlobal::characterSize.y / 2;

            for (auto it = particles.begin(); it != particles.end();) {
                auto& particle = *it;

                particle->update(dt, playerPosition);


                if (particle->remove()) {
                    it = particles.erase(it);
                } else {
                    ++it;
                }
            }
        }

        std::list<std::shared_ptr<Particle>>& getParticlesList() {
            return particles;
        }
        
        void render(sf::RenderTarget &target) {
            for(const auto &particle : particles) {
                particle->render(target);
            }
        }
    private:
        std::list<std::shared_ptr<Particle>> particles;
};