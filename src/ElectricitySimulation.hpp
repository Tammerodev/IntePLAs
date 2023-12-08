#pragma once
#include "ElectricComponent.hpp"

class ElectricitySimulation {
    public:
        void load();

        void addElement(const std::shared_ptr<ElectricComponent>);

        void update(ChunkIndexer&);

    protected:
        std::list<std::shared_ptr<ElectricComponent>> components; 
};