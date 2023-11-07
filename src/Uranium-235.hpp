#pragma once
#include "RadioactiveElement.hpp"
#include "Chunk.hpp"

class Uranium235 : public RadioactiveElement {
    public:
        Uranium235(int x, int y) {
            this->x = x;
            this->y = y;
        }

        void update(ChunkIndexer &world) {
            radiation_pulse = radiation_strength * (math::randIntInRange(0, 100) < radiation_frequency);
        }

        int getRadiationStrength() {
            return radiation_pulse;
        }
    private:
        int radiation_strength = 200;
        int radiation_frequency = 40;
        int radiation_pulse = 0;
};