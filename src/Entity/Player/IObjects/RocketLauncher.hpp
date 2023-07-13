#pragma once 
#include "Gun.hpp"

class RocketLauncher : public Gun {
    public:
        RocketLauncher(VoxelManager &vx) : Gun(vx, "res/img/Tool/bullet.png", "res/img/Tool/rocket_launcher.png",60) {}
    private:
};