#pragma once
#include "RadioactiveElement.hpp"
#include "Chunk.hpp"
#include "ChunkIndexerVoxelContainer.hpp"
#include "math.hpp"

class RadioactiveWaste : public RadioactiveElement {
    public:
        RadioactiveWaste(int x, int y) : RadioactiveElement() {
            this->x = x;
            this->y = y;

            half_life = 100;
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
            return isDecayed();
        }

    private:
        int radiation_strength = 1500;
        int radiation_frequency = 60;
        int radiation_pulse = 0;

        bool remove = false;
};