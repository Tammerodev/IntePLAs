#pragma once
#include <iostream>

struct VoxelGroupPropeties {
    VoxelGroupPropeties() {
        
    }

    VoxelGroupPropeties(const unsigned int nutr, const std::string inf = "") : nutrition(nutr), info(inf) {
        
    }

    unsigned int nutrition = 0;
    std::string info = "";
};