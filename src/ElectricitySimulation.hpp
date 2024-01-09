#pragma once
#include "ElectricComponent.hpp"
#include "debug_globals.hpp"
#include "common.hpp"

class ElectricitySimulation {
    public:
        void load();

        void addElement(const std::shared_ptr<ElectricComponent>);

        void update(ChunkIndexer&);

    protected:
        std::list<std::shared_ptr<ElectricComponent>> components; 
};