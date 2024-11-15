#pragma once
#include <cstdint>

struct MaterialGain {
    int16_t amount;
};

struct MaterialPack {
    std::map<int, MaterialGain> materials;

    bool addElementOfType(const int type, const int amount) {
        if(type == 0) return true;

        materials[type].amount += amount;
        return true;
    }
};
