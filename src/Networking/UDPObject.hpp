#pragma once
#include <SFML/Network/Packet.hpp>

class UDPObject {
    public:
        virtual void init() = 0;
        virtual void send(sf::Packet &) = 0;
        virtual void receive(sf::Packet &) = 0;
    private:

};