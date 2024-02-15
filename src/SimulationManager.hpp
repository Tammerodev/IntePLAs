#pragma once
#include "ElectricitySimulation.hpp"
#include "Led.hpp"

class SimulationManager {
    public: 
        void load() {
            es.load();
        }

        void addElementToES(std::shared_ptr<ElectricComponent> component) {
            es.addElement(component);
        }

        void updateAll(ChunkIndexer& world) {
            es.update(world);

            // TODO es.remove
        }

        void drawAll(sf::RenderTarget& target) {
            es.render(target);
        }


    private:
        ElectricitySimulation es;
};