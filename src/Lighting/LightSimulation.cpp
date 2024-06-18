#include "LightSimulation.hpp"

void LightSimulation::load() {  

}

void LightSimulation::update(ChunkIndexer &world, ShaderEffect& eff) {
    if(targ == nullptr) return;

    LightGlobal::check_sources(world);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L)) {
        LightGlobal::add_source(sf::Vector2i(debug_globals::player_position), LightGlobal::LightElement(25, sf::Color::White));
    }


    const int size = std::clamp(LightGlobal::source_positions.size(), (size_t)0, (size_t)50);
    int index = 0;

    for(auto &light : LightGlobal::source_positions) {
        if(index > size - 1) break;

        loginf(light.first.x, "<x --_-- y>", light.first.y);

        const sf::Vector2f pos = sf::Vector2f(light.first);

        eff.desaturate_shader.o_setUniform("lights["+ std::to_string(index) +"]", 
            sf::Glsl::Vec4(
               pos.x, pos.y, light.second.intensity, light.second.color.toInteger()));
    }
}

void LightSimulation::render(sf::RenderTarget &target) {
   targ = &target;
}
