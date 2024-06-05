#pragma once
#include "Flammable.hpp"

class Wood : public Flammable {
    public:
        Wood(int x, int y) : Flammable(x, y) {
            ignition_temp = 350;
            breakdown_temp = 2000;
        }
    private:
};