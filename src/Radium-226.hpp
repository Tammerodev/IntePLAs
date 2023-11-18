#pragma once
#include "RadioactiveElement.hpp"
#include "Chunk.hpp"
#include "math.hpp"

class Radium226 : public RadioactiveElement {
    public:
        Radium226(int x, int y) {
            this->x = x;
            this->y = y;
        }

        void update(ChunkIndexer &world) {
            radiation_pulse = radiation_strength * (math::randIntInRange(0, 100) < radiation_frequency);

            // Check if voxel doesnt exist
            remove = world.getVoxelAt(x, y).value == 0;
        }

        int getRadiationStrength() {
            return radiation_pulse;
        }

        bool clear() {
            return remove;
        }

    private:
        int radiation_strength = 1200;
        int radiation_frequency = 100;
        int radiation_pulse = 0;

        bool remove = false;
};