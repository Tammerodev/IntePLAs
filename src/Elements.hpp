#pragma once
#include <SFML/Graphics/Color.hpp>
#include <cstdint>

namespace elm {
    const sf::Color Carbon {74,74,74};
    const sf::Color Lithium {119,120,115};
    const sf::Color Magnesium {186,186,166};
    const sf::Color Sodium {119,142,125};
    const sf::Color Aluminium {209,213,216};
    const sf::Color Silicon {117,121,139};
    const sf::Color Copper {186,140,106};
    const sf::Color Titanium {142,129,149};
    const sf::Color Lead {104,102,107};
    const sf::Color Water {30,129,176, 100};
    const sf::Color Nitroglycerin {159, 8, 23, 200};
    const sf::Color Chlorine {239, 255, 61, 60};



    const uint8_t ValCarbon = 2;
    const uint8_t ValLithium = 3;
    const uint8_t ValMagnesium = 4;
    const uint8_t ValSodium = 5;
    const uint8_t ValAluminium = 6;
    const uint8_t ValSilicon = 7;
    const uint8_t ValCopper = 8;
    const uint8_t ValTitanium = 9;
    const uint8_t ValLead = 10;
    const uint8_t ValWater = 11;
    const uint8_t ValNitroglycerin = 12;
    const uint8_t ValChlorine = 13;



    const unsigned short defaultMaxTemp = 1000;
    const unsigned short carbonMaxTemp = 3550;
    const unsigned short lithiumMaxTemp = 180;
    const unsigned short magnesiumMaxTemp = 650;
    const unsigned short sodiumMaxTemp = 98;
    const unsigned short aluminiumMaxTemp = 660;
    const unsigned short siliconMaxTemp = 1410;
    const unsigned short copperMaxTemp = 1085;
    const unsigned short titaniumMaxTemp = 1668;
    const unsigned short leadMaxTemp = 327;
    const unsigned short waterMaxTemp = 100;
    const unsigned short nitroglycerinMaxTemp = 100;



    const unsigned short defaultAmbientDissipation = 1;
    const unsigned short carbonAmbientDissipation = 1;
    const unsigned short lithiumAmbientDissipation = 1;
    const unsigned short magnesiumAmbientDissipation = -1;
    const unsigned short sodiumAmbientDissipation = 1;
    const unsigned short aluminiumAmbientDissipation = 1;
    const unsigned short siliconAmbientDissipation = 1;
    const unsigned short copperAmbientDissipation = 1;
    const unsigned short titaniumAmbientDissipation = 1;
    const unsigned short leadAmbientDissipation = 1;
    const unsigned short waterAmbientDissipation = 8;
    const unsigned short nitroglycerinAmbientDissipation = 8;


    const unsigned short lithiumExplosion = 100;
    const unsigned short lithiumExplosionTemp = 100;
    const unsigned short nitroglycerinExplosionTemp = 200;
    const unsigned short nitroglycerinExplosion = 100;

    const unsigned short sodiumExplosion = 10;
    const unsigned short sodiumExplosionTemp = 500;


    static const unsigned short getMaxTempFromType(const uint8_t type) {
        switch (type)
        {
        case 2:
            return carbonMaxTemp;
        case 3:
            return lithiumMaxTemp;
        case 4:
            return magnesiumMaxTemp;
        case 5:
            return sodiumMaxTemp;
        case 6:
            return aluminiumMaxTemp;
        case 7:
            return siliconMaxTemp;
        case 8:
            return copperMaxTemp;
        case 9:
            return titaniumMaxTemp;
        case 10:
            return leadMaxTemp;
        case 11:
            return waterMaxTemp;
        case ValNitroglycerin:
            return waterMaxTemp;

        default:
            return defaultMaxTemp;
        }
    }

    static const unsigned short getAmbientDissipationFromType(const uint8_t type) {
        switch (type)
        {
        case 2:
            return carbonAmbientDissipation;
        case 3:
            return lithiumAmbientDissipation;
        case 4:
            return magnesiumAmbientDissipation;
        case 5:
            return sodiumAmbientDissipation;
        case 6:
            return aluminiumAmbientDissipation;
        case 7:
            return siliconAmbientDissipation;
        case 8:
            return copperAmbientDissipation;
        case 9:
            return titaniumAmbientDissipation;
        case 10:
            return leadAmbientDissipation;
        case 11:
            return waterAmbientDissipation;
        case ValNitroglycerin:
            return nitroglycerinAmbientDissipation;
        default:
            return defaultAmbientDissipation;
        }
    }

    static const bool isFluid(const uint8_t type) {
        return (type > 10);
    }
}
