#pragma once
#include <SFML/Network.hpp>
#include "UDPObject.hpp"
#include <iostream>

class Server : public UDPObject {
public:
    sf::UdpSocket socket;

    bool init() {
        ip = sf::IpAddress::getLocalAddress();

        if(socket.bind(5401) != sf::UdpSocket::Status::Done) {
            std::cerr << "UDP Socket couldnt bind at port " << port << '\n';
            return false;
        }
        socket.setBlocking(false);
        return true;
    }

    void receive(sf::Packet &packet) {
        sf::IpAddress remote_ip;
        unsigned short port;

        socket.receive(packet, remote_ip, port);
    }

    void send(sf::Packet &packet) {
        while(socket.send(packet, ip, 5400) != sf::UdpSocket::Status::Done);
    }

private:
    unsigned short port;
};