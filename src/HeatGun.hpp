#pragma once 
#include "Gun.hpp"

class HeatGun : public Gun {
    public:
        HeatGun(VoxelManager &vx) : Gun(vx, "res/img/Tool/fire.png", "res/img/Tool/Heatgun.png",60, false, 10, true) {}
};