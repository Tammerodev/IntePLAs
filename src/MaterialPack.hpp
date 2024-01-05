#pragma once
#include <cstdint>

struct MaterialPack {
    int64_t carbon = 0;
    int64_t lithium = 0;
    int64_t magnesium = 0;
    int64_t sodium = 0;
    int64_t aluminium = 0;
    int64_t silicon = 0;
    int64_t copper = 0;
    int64_t titanium = 0;
    int64_t lead = 0;

    void addElementOfType(const int type, const int amount) {
        switch (type)
        {
            case elm::ValCarbon: carbon += amount; return;
            case elm::ValLithium: lithium += amount; return;
            case elm::ValMagnesium: magnesium += amount; return;
            case elm::ValSodium: sodium += amount; return;
            case elm::ValAluminium: aluminium += amount; return;
            case elm::ValSilicon: silicon += amount; return;
            case elm::ValCopper: copper += amount; return;
            case elm::ValTitanium: titanium += amount; return;
            case elm::ValLead: lead += amount; return;
    
        default:
            break;
        }
    }
};
