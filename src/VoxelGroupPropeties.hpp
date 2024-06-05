#pragma once


struct VoxelGroupPropeties {
    VoxelGroupPropeties() {
        
    }

    VoxelGroupPropeties(unsigned int nutr) : nutrition(nutr) {
        
    }

    unsigned int nutrition = 0;
};