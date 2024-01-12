#pragma once
#include "ParticleSimulation.hpp"
#include "FireParticle.hpp"
#include "math.hpp"

namespace FireGlobal {
    inline std::list<std::pair<sf::Vector2i, int>> sources;
    inline const int source_lifetime = 1;

    inline void add_source(const sf::Vector2i &pos) {
        sources.push_back(std::pair(pos, source_lifetime));
    }

}

class FireEffectManager {
    public:

        void update(ParticleSimulation& particleSimulation);
    private:
};