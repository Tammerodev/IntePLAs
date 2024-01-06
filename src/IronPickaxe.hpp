#pragma once
#include "Pickaxe.hpp"

class IronPickaxe : public Pickaxe {
    public:
        IronPickaxe(VoxelManager &vx) : Pickaxe(vx, 3.f, 10) {

        }

        
};