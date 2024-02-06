#pragma once
#include <string>
#include <list>

enum class MobType;

struct VegetationInfo {
    int amount = 0;
    std::string filepath = ""; 

    int width_of_part = 16;
    int height_of_part = 16;

    int parts_on_x = 7;
    int offset_up = 6;
};

struct MobSpawnInfo {
    int amount = 0;
    
    std::vector<std::pair<MobType, int>> spawns;
};

struct BiomeInfo {
    VegetationInfo vegetationInfo;
    MobSpawnInfo mobInfo;
};