#include "LightSimulation.hpp"

void LightSimulation::load() {  

}

void LightSimulation::update(ChunkIndexer &world, ShaderEffect& eff) {
    LightGlobal::check_sources(world);

    const int size = std::clamp(LightGlobal::source_positions.size(), (size_t)0, (size_t)50);
    int index = 0;

    for(auto &light : LightGlobal::source_positions) {
        if(index > size - 1) break;

        loginf(light.first.x, "<x --_-- y>", light.first.y);

        eff.desaturate_shader.o_setUniform("lights["+ std::to_string(index) +"]", 
            sf::Glsl::Vec4(
                light.first.x, light.first.y, light.second.intensity, light.second.color.toInteger()));
    }
}

void LightSimulation::render(sf::RenderTarget &target) {
    
}
