#include "PlaceItem.hpp"


class DebugPlacer : public PlaceItem {
    public:
        DebugPlacer(VoxelManager &vx) : PlaceItem(vx,"res/img/Tool/DebugPlacer.png", "res/img/Tool/bullet.png") {}
    private:
};