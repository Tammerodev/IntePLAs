#pragma once
#include "Gas.hpp"

class Chlorine : public Gas {
    public:
        Chlorine(int xx, int yy) : Gas(xx, yy) {
            value = elm::ValChlorine;
            color = elm::Chlorine;
        }
};