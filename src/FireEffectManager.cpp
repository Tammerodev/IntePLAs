#include "FireEffectManager.hpp"

void FireEffectManager::update(ParticleSimulation &particleSimulation){
    for(auto it = FireGlobal::sources.begin(); it != FireGlobal::sources.end();) {
        it->second--;

        const auto particle = std::make_shared<FireParticle>(
            sf::Vector2f(it->first),
            sf::Vector2f(math::randFloat(), math::randFloat()),
            sf::Color::Red,
            math::randFloat() + 1.0f
        );

        particleSimulation.addParticle(particle);

        if(it->second < 0) {
            it = FireGlobal::sources.erase(it);
        } else {
            ++it;
        }
    }
}