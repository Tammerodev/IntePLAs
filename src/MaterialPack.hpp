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

    int64_t dummy = 0;

    int64_t& accessToElementOfType(const int type) {
        switch (type)
        {
            case VoxelValues::CARBON:         return carbon;
            case VoxelValues::LITHIUM:        return lithium;
            case VoxelValues::MAGNESIUM:      return magnesium;
            case VoxelValues::SODIUM:         return sodium;
            case VoxelValues::ALUMINIUM:      return aluminium;
            case VoxelValues::SILICON:        return silicon;
            case VoxelValues::COPPER:         return copper;
            case VoxelValues::TITANIUM:       return titanium;
            case VoxelValues::LEAD:           return lead;
    
        default:
            return dummy;
        }
    }

    bool addElementOfType(const int type, const int amount) {
        int64_t &element = accessToElementOfType(type);
        element += amount;

        if(element < 0) {
            element = 0;
            return false;
        }

        return true;
    }
};
