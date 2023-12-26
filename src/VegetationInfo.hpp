#pragma once
#include <string>

struct VegetationInfo {
    int amount = 0;
    std::string filepath = ""; 

    int width_of_part = 16;
    int height_of_part = 16;

    int parts_on_x = 7;
    int offset_up = 6;
};