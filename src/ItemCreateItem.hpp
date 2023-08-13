#pragma once
#include "PlaceItem.hpp"

class ItemCreateItem : public PlaceItem {
    public:
    ItemCreateItem(VoxelManager &vx) : PlaceItem(vx,"res/img/Tool/DebugPlacer.png", "res/img/Tool/bullet.png") {}
};