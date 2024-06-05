#pragma once
#include "ElectricitySimulation.hpp"
#include "Led.hpp"
#include "Lighting/LightSimulation.hpp"
#include "RadiationSearch.hpp"

class SimulationManager {
    public: 
        void load(ChunkIndexer &world, sf::Vector2f *pos) {
            es.load();
            ls.load();

            RadiationSearch::launch(world, pos);
        }

        void addElementToES(std::shared_ptr<ElectricComponent> component) {
            es.addElement(component);
        }

        void updateAll(ChunkIndexer& world, ShaderEffect& eff) {
            es.update(world);
            ls.update(world, eff);

            RadiationSearch::update();
            // TODO es.remove
        }

        void drawAll(sf::RenderTarget& target, sf::RenderTarget& renderTex) {
            es.render(target);
            ls.render(renderTex);
        }


    private:
        ElectricitySimulation es;
        LightSimulation ls;
};