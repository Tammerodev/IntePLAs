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

    const uint8_t ValCarbon = 2;
    const uint8_t ValLithium = 3;
    const uint8_t ValMagnesium = 4;
    const uint8_t ValSodium = 5;
    const uint8_t ValAluminium = 6;
    const uint8_t ValSilicon = 7;
    const uint8_t ValCopper = 8;
    const uint8_t ValTitanium = 9;
    const uint8_t ValLead = 10;

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
        default:
            return defaultAmbientDissipation;
        }
    }
}
