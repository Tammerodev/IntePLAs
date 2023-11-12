#pragma once
#include "UDPObject.hpp"
#include <SFML/Network.hpp>

class Client : UDPObject {
public:
    sf::UdpSocket socket;
    sf::IpAddress ip;

    void init() {
        ip = sf::IpAddress::getLocalAddress();
    }

    void send(sf::Packet &packet) {
        socket.send(packet, ip, 5401);
    }

    void receive(sf::Packet &packet) {

    }
};