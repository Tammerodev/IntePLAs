#include "FireEffectManager.hpp"

void FireEffectManager::update(ParticleSimulation &particleSimulation){
    for(auto it = FireGlobal::sources.begin(); it != FireGlobal::sources.end();) {
        const auto particle = std::make_shared<FireEffectParticle>(
            sf::Vector2f(it->first),
            sf::Vector2f(math::randFloat() - 0.5f, -(0.3f + math::randFloat() / 10.f)),
            sf::Color(255, 255, 255),
            math::randFloat() + 1.0f,
            it->second
        );

        particleSimulation.addParticle(particle);
        ++it;
    }

    FireGlobal::sources.clear();

}