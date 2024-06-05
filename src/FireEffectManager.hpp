#pragma once
#include "ParticleSimulation.hpp"
#include "Particle/FireEffectParticle.hpp"
#include "math.hpp"

namespace FireGlobal {
    inline std::list<std::pair<sf::Vector2i, int>> sources;
    
    inline void add_source(const sf::Vector2i &pos, const int e) {
        sources.push_back(std::pair(pos, e));
    }

}

class FireEffectManager {
    public:
        void update(ParticleSimulation& particleSimulation);
    private:
};