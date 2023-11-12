#pragma once
#include <SFML/Graphics.hpp>
#include "UDPObject.hpp"
#include <iostream>

class NetworkingManagerUDP {
    public:
        static sf::Packet serializeImage(const sf::Image &image, const sf::Vector2i& chunk_pos) {
            sf::Packet packet;
            packet << image.getSize().x << image.getSize().y;
            packet << chunk_pos.x << chunk_pos.y;


            packet.append(image.getPixelsPtr(), image.getSize().x * image.getSize().y * 4);

            return packet;
        }

        static void deserializeImage(sf::Packet &received_packet, sf::Image&img, sf::Vector2i&chunk_pos) {
            sf::Vector2i size;

            received_packet >> size.x >> size.y;
            received_packet >> chunk_pos.x >> chunk_pos.y;

            std::vector<sf::Uint8> pixels;
            pixels.resize(size.x * size.y * 4); // Assuming RGBA format
        

            for (std::size_t i = 0; i < pixels.size(); ++i) {

                if (!(received_packet >> pixels[i])) {
                    std::cerr << "Failed to extract pixel data from packet" << std::endl;
                    return;
                }
            }

            img.create(size.x, size.y, pixels.data());
        }
};