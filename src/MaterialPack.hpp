#pragma once
#include <cstdint>

struct MaterialPack {
    int64_t carbon = 0;
    int64_t lithium = 0;
    int64_t magnesium = 0;
    int64_t sodium = 0;
    int64_t aluminium = 0;
    int64_t silicon = 0;
    int64_t copper = 0;
    int64_t titanium = 0;
    int64_t lead = 0;

    void addElementOfType(const int type, const int amount) {
        switch (type)
        {
            case VoxelValues::CARBON: carbon += amount; return;
            case VoxelValues::LITHIUM: lithium += amount; return;
            case VoxelValues::MAGNESIUM: magnesium += amount; return;
            case VoxelValues::SNOW: sodium += amount; return;
            case VoxelValues::ALUMINIUM: aluminium += amount; return;
            case VoxelValues::SILICON: silicon += amount; return;
            case VoxelValues::COPPER: copper += amount; return;
            case VoxelValues::TITANIUM: titanium += amount; return;
            case VoxelValues::LEAD: lead += amount; return;
    
        default:
            break;
        }
    }
};
