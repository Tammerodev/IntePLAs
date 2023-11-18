#include "ClientUDP.hpp"
#include "ServerUDP.hpp"

#include "NetworkingManagerUDP.hpp"

#include <iostream>

Client client;
Server server;

int main() {    

    char select;

    std::cout << "(s)erver, (c)lient";
    std::cin >> select;

    if(select == 'c') {
        client.init();
        sf::Image img;
        img.create(64, 64, sf::Color::Red);

        sf::Packet packet(NetworkingManagerUDP::serializeImage(img, sf::Vector2i(0,0)));

        client.send(packet);
    }   
    if(select == 's') {
        
        server.init();
        sf::Image rec_img;
        sf::Vector2i pos;

        sf::Packet packet;
a:
        server.receive(packet);

        std::cout << packet.getData();

        if(packet.getData() == NULL) goto a;

        std::cout << "aaaaa";

        NetworkingManagerUDP::deserializeImage(packet, rec_img, pos);

        sf::Texture tx;

        tx.loadFromImage(rec_img);
        sf::Sprite spr(tx);

        sf::RenderWindow window(sf::VideoMode(600,600), "Server");
        sf::Event ev;
        while(true) {
            window.clear();
            window.draw(spr);
            spr.setScale(10, 10);

            while(window.pollEvent(ev)) {

            }

            window.display();
        }

    }
}