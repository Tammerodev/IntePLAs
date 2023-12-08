#include "ElectricitySimulation.hpp"

void ElectricitySimulation::load() {
    
}

void ElectricitySimulation::addElement(const std::shared_ptr<ElectricComponent> component) {
    components.push_back(component);
}

void ElectricitySimulation::update(ChunkIndexer& world) {
    for(auto &component : components) {
        component->update(world);
    }
}
