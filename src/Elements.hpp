#pragma once
#include <SFML/Graphics/Color.hpp>
#include <cstdint>
#include <string>

struct VoxelInfo {
    VoxelInfo(uint8_t val, sf::Color col, const unsigned short mt, const std::string nm) {
        value = val;
        color = col;
        max_temp = mt;
        name = nm;
    }


    uint8_t value = 0;
    sf::Color color;
    unsigned short max_temp = 0;
    std::string name = "";
};

enum VoxelValues {
    DEFAULT = 0,
    CARBON = 2,
    LITHIUM = 3,
    MAGNESIUM = 4,
    SODIUM = 5,
    ALUMINIUM = 6,
    SILICON = 7,
    COPPER = 8,
    TITANIUM = 9,
    LEAD = 10,
    WATER = 11,
    NITROGLYCERIN = 12,
    CHLORINE = 13,
    URANIUM235 = 20,
    RADIUM226 = 21,
    SAND = 30,
    SNOW = 31,
    GLASS = 32,
    ACID = 14
};

namespace elm {
    //  Voxel type definitions:    -Type-          -Color RGB-       -Max Temp-  -Name string-

    const VoxelInfo Default         {0, sf::Color(0, 0, 0),             1000,   ""                };

    const VoxelInfo Carbon          {2,  sf::Color(74,74,74),           3550,   "Carbon"          };
    const VoxelInfo Lithium         {3,  sf::Color(119,120,115),        180,    "Lithium"         };
    const VoxelInfo Magnesium       {4,  sf::Color(186,186,166),        650,    "Magnesium"       };
    const VoxelInfo Sodium          {5,  sf::Color(119,142,125),        98,     "Sodium"          };
    const VoxelInfo Aluminium       {6,  sf::Color(209,213,216),        660,    "Aluminium"       };
    const VoxelInfo Silicon         {7,  sf::Color(117,121,139),        1410,   "Silicon"         };
    const VoxelInfo Copper          {8,  sf::Color(186,140,106),        1085,   "Copper"          };
    const VoxelInfo Titanium        {9,  sf::Color(142,129,149),        1668,   "Titanium"        };
    const VoxelInfo Lead            {10, sf::Color(104,102,107),        327,    "Lead"            };
    const VoxelInfo Water           {11, sf::Color(30,129,176, 100),    100,    "Water"           };
    const VoxelInfo Nitroglycerin   {12, sf::Color(159, 8, 23, 200),    100,    "Nitroglycerin"   };
    const VoxelInfo Chlorine        {13, sf::Color(239, 255, 61, 60),   10000,  "Chlorine"        };
    const VoxelInfo Uranium235      {20, sf::Color(71, 201, 75),        1132,   "Uranium - 235"   };
    const VoxelInfo Radium226       {21, sf::Color(31, 71, 57),         1132,   "Radium - 226"    };
    const VoxelInfo Sand            {30, sf::Color(168, 173, 26),       10000,  "Sand"            };
    const VoxelInfo Snow            {31, sf::Color(211, 235, 230),      10,     "Snow"            };
    const VoxelInfo Acid            {14, sf::Color(68, 255, 0),         100,    "Acid"            };
    const VoxelInfo Glass           {32, sf::Color(68, 255, 0),         1000,   "Glass"           };

    const unsigned short lithiumExplosion = 100;
    const unsigned short lithiumExplosionTemp = 100;
    const unsigned short nitroglycerinExplosionTemp = 200;
    const unsigned short nitroglycerinExplosion = 100;

    const unsigned short sodiumExplosion = 10;
    const unsigned short sodiumExplosionTemp = 500;

    static const VoxelInfo& getInfoFromType(const int type) {
        switch(type) {
            case CARBON:
                return Carbon;
            case LITHIUM:
                return Lithium;
            case MAGNESIUM:
                return Magnesium;
            case SODIUM:
                return Sodium;
            case ALUMINIUM:
                return Aluminium;
            case SILICON:
                return Silicon;
            case COPPER:
                return Copper;
            case TITANIUM:
                return Titanium;
            case LEAD:
                return Lead;
            case WATER:
                return Water;
            case NITROGLYCERIN:
                return Nitroglycerin;
            case CHLORINE:
                return Chlorine;
            case SAND:
                return Sand;
            case SNOW:
                return Snow;           
            case GLASS:
                return Glass;
            case URANIUM235:
                return Uranium235;
            case RADIUM226: 
                return Radium226;

            default:
                return Default;
        }
    }

    static const bool isFluid(const uint8_t type) {
        return (type > 10);
    }
}
