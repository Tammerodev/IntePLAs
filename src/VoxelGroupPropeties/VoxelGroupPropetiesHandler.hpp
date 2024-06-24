#pragma once
#include "../VoxelGroup.hpp"
#include "../UIStateChangeRequest.hpp"

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

        if(Controls::collect() && propeties.info != "") {
            if(propeties.info.at(0) == 'U') {
                if(propeties.info.at(1) == 'm')
                    UIStateChangeRequest::newState = "i";
            }
        }

        return false;
    }
}