#pragma once
#include "common.hpp"

namespace VoxelGroupIDGenerator {
    inline int _id = 1;


    static const int getNextID() {
        _id++;
        return _id;
    }

    static const bool checkID(int __id) {
        if(!__id) {
            prndd("Voxel group id is set to 0");
            return false;
        }

        return true;
    }
}