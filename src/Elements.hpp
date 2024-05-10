#pragma once
#include <SFML/Graphics/Color.hpp>
#include <cstdint>
#include <string>

struct VoxelInfo {
    VoxelInfo(uint8_t val, const uint8_t str, sf::Color col, const unsigned short mt, const float rd, const std::string nm) {
        value = val;
        strength = str;
        color = col;
        max_temp = mt;
        name = nm;
        radiation = rd;
    }


    uint8_t value = 0;
    uint8_t strength = 1;
    float radiation = 0.1f;
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
    GLASS = 11,
    WOOD = 12,

    MIRROR = 20,

    SAND = 30,
    SNOW = 31,

    WATER = 50,
    NITROGLYCERIN = 51,
    ACID = 52,
    BLOOD = 53,
    MANGANESEHEPTOXIDE = 54,

    STEAM = 60,
    CHLORINE = 61,
    BUTANE = 62,

    URANIUM235 = 100,
    RADIUM226 = 101,

    OSCILLATOR = 200,
    TRANSISTORDON = 201,
    TRANSISTORDOFF = 202,

    SWITCH = 203,
};

namespace elm {
    //  Voxel type definitions:    -Type-          -Color RGB-       -Max Temp-  -Name string-

    const VoxelInfo Default         {0, 1, sf::Color(0, 0, 0),             1000, 0,  ""                };

    const VoxelInfo Carbon          {VoxelValues::CARBON,       2,  sf::Color(74,74,74),           3550, 0.0f,   "Carbon"          };
    const VoxelInfo Lithium         {VoxelValues::LITHIUM,      3,  sf::Color(119,120,115),        180,  0.02f,  "Lithium"         };
    const VoxelInfo Magnesium       {VoxelValues::MAGNESIUM,    4,  sf::Color(186,186,166),        650,  0.01f,   "Magnesium"       };
    const VoxelInfo Sodium          {VoxelValues::SODIUM,       1,  sf::Color(119,142,125),        98,   0.02f,   "Sodium"          };
    const VoxelInfo Aluminium       {VoxelValues::ALUMINIUM,    4,  sf::Color(209,213,216),        660,  0.0f,   "Aluminium"       };
    const VoxelInfo Silicon         {VoxelValues::SILICON,      1,  sf::Color(117,121,139),        1410, 0.0f,   "Silicon"         };
    const VoxelInfo Copper          {VoxelValues::COPPER,       4,  sf::Color(186,140,106),        1085, 0.01f,   "Copper"          };
    const VoxelInfo Titanium        {VoxelValues::TITANIUM,     10, sf::Color(142,129,149),        1668, 0.02f,   "Titanium"        };
    const VoxelInfo Lead            {VoxelValues::LEAD,         8,  sf::Color(104,102,107),        327,  0.02f,   "Lead"            };
    const VoxelInfo Wood            {VoxelValues::WOOD,         1,  sf::Color(61, 36, 36),         327,  0.1f,   "Wood"            };
    const VoxelInfo Mirror          {VoxelValues::MIRROR,       1,  sf::Color(61, 61, 61),         1000, 0.0f,   "Mirror"            };
    
    
    const VoxelInfo Water           {VoxelValues::WATER,        14, sf::Color(30,129,176, 100),    100,  0.001f,   "Water"           };
    const VoxelInfo Nitroglycerin   {VoxelValues::NITROGLYCERIN,14, sf::Color(159, 8, 23, 200),    100,  0.0f,   "Nitroglycerin"   };
    const VoxelInfo Chlorine        {VoxelValues::CHLORINE,     99, sf::Color(239, 255, 61, 60),   10000,0.0f,   "Chlorine"        };
    const VoxelInfo Acid            {VoxelValues::ACID,         14, sf::Color(68, 255, 0),         100,  0.0f,   "Acid"            };
    const VoxelInfo Blood           {VoxelValues::BLOOD,        14, sf::Color(255, 49, 20, 200),   100,  0.0f,   "Blood"           };
  const VoxelInfo ManganeseHeptoxide{VoxelValues::MANGANESEHEPTOXIDE, 14, sf::Color(255, 49, 20, 200),   100, 0.0f,   "Manganese Heptoxide"           };


    const VoxelInfo Uranium235      {VoxelValues::URANIUM235,   10, sf::Color(71, 201, 75),        1132,   25.0f, "Uranium - 235"   };
    const VoxelInfo Radium226       {VoxelValues::RADIUM226,    1,  sf::Color(31, 71, 57),         1132,   125.f, "Radium - 226"    };
    const VoxelInfo Sand            {VoxelValues::SAND,         14, sf::Color(168, 173, 26),       10000,  0.06f, "Sand"            };
    const VoxelInfo Snow            {VoxelValues::SNOW,         1,  sf::Color(211, 235, 230),      10,     0.0f, "Snow"            };
    const VoxelInfo Glass           {VoxelValues::GLASS,        1,  sf::Color(125, 125, 100, 100), 1000,   0.01f, "Glass"           };

    const VoxelInfo Steam           {VoxelValues::STEAM,        99, sf::Color(68, 255, 0),         1000,   0.0f, "Steam"           };
    const VoxelInfo Butane          {VoxelValues::BUTANE,      100, sf::Color(20, 20, 20, 60),     420,    0.0f, "Butane"          };

    const VoxelInfo Oscillator      {VoxelValues::OSCILLATOR,   3,  sf::Color(255, 87, 244),       1000,   0.0f, "Oscillator"      };
    const VoxelInfo TransistorDon   {VoxelValues::TRANSISTORDON,3,  sf::Color(107, 13, 55),        1000,   0.0f, "Transistor Default on"      };
    const VoxelInfo TransistorDoff  {VoxelValues::TRANSISTORDOFF,3, sf::Color(107, 100, 55),       1000,   0.0f, "Transistor Default off"      };
 
    const VoxelInfo Switch          {VoxelValues::SWITCH,       3,  sf::Color(28, 94, 24),         1000,   0.0f, "Switch"          };



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
            case WOOD:
                return Wood;    

            case MIRROR:
                return Mirror;

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
            case ACID:
                return Acid;
            case WATER:
                return Water;
            case NITROGLYCERIN:
                return Nitroglycerin;
            case CHLORINE:
                return Chlorine;
            case BLOOD:
                return Blood;
            case MANGANESEHEPTOXIDE:    
                return ManganeseHeptoxide;
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
            case STEAM:
                return Steam;
            case BUTANE:
                return Butane;

            case OSCILLATOR:
                return Oscillator;

            case TRANSISTORDON:
                return TransistorDon;

            case TRANSISTORDOFF:
                return TransistorDoff;

            case SWITCH:
                return Switch;

            default:
                return Default;
        }
    }

}
