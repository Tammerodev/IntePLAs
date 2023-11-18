#pragma once
#include <SFML/Network.hpp>

class UDPObject {
    public:
        virtual bool init() = 0;
        virtual void send(sf::Packet &) = 0;
        virtual void receive(sf::Packet &) = 0;

        const sf::IpAddress& getIP() {
            return ip;
        }
        
    protected:
        sf::IpAddress ip;
};