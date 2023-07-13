#include "PlaceItem.hpp"


class DebugPlacer : public PlaceItem {
    public:
        DebugPlacer(VoxelManager &vx) : PlaceItem(vx,"res/img/Tool/DebugPlacer.png", "res/img/Tool/debug.png") {}
    private:
};