#include "LightSimulation.hpp"

void LightSimulation::load() {  

}

void LightSimulation::update(ChunkIndexer &world) {
    LightGlobal::check_sources(world);
}

void LightSimulation::render(sf::RenderTarget &target) {
    for(auto &source : LightGlobal::source_positions) {
        sf::CircleShape lightCircle;

        sf::Color c = source.second.color;
        c.a = source.second.intensity; 

        lightCircle.setFillColor(c);
        lightCircle.setRadius(source.second.intensity / 5.f);

        lightCircle.setPosition(sf::Vector2f(source.first));
        lightCircle.setOrigin(lightCircle.getRadius(), lightCircle.getRadius());

        target.draw(lightCircle);
    }
}
