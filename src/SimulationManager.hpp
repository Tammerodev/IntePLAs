#pragma once
#include "ElectricitySimulation.hpp"
#include "Led.hpp"
#include "Lighting/LightSimulation.hpp"

class SimulationManager {
    public: 
        void load() {
            es.load();
            ls.load();
        }

        void addElementToES(std::shared_ptr<ElectricComponent> component) {
            es.addElement(component);
        }

        void updateAll(ChunkIndexer& world) {
            es.update(world);
            ls.update(world);
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