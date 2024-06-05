#pragma once
#include "VoxelGroup.hpp"

namespace VoxelGroupPropetiesHandler {
    inline bool handle(std::shared_ptr<VoxelGroup> vg, Player& player) {
        auto propeties = vg->getPropeties();

        if(Controls::collect() && propeties.nutrition != 0) {
            const float distance = math::distance(player.getHeadPosition(), vg->getPosition());
            
            if(distance < 35) {
                player.getHungerManager().eat(propeties.nutrition);
                return true;
            }
        }

        return false;
    }
}