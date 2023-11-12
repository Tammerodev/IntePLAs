#pragma once
#include <SFML/Network.hpp>
#include "UDPObject.hpp"

class Server : UDPObject {
public:
    sf::UdpSocket socket;
    sf::IpAddress ip;

    void init() {
        ip = sf::IpAddress::getLocalAddress();

        socket.bind(5401);
    }

    void receive(sf::Packet &packet) {
        sf::IpAddress remote_ip;
        unsigned short port;

        socket.receive(packet, remote_ip, port);
    }

    void send(sf::Packet &packet) {
        
    }
};