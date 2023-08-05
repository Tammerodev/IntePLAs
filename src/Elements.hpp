#pragma once
#include <SFML/Graphics/Color.hpp>

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
}
