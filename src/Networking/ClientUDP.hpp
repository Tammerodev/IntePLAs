#pragma once
#include "UDPObject.hpp"
#include <SFML/Network.hpp>

class Client : public UDPObject {
public:
    sf::UdpSocket socket;


    bool init() {
        ip = sf::IpAddress::getLocalAddress();
        socket.bind(5400);
        return true;
    }

    void send(sf::Packet &packet) {
        while(socket.send(packet, ip, 5401) != sf::UdpSocket::Status::Done);
    }

    void receive(sf::Packet &packet) {
        sf::IpAddress remote_ip;
        unsigned short port;

        socket.receive(packet, remote_ip, port);
    }
};