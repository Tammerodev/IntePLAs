#pragma once 
#include "VoxelManager.hpp"

class ClientManager {
    public:
        Client client;

        void update(ChunkIndexer &world) {
            sf::Packet packet_received;
            client.receive(packet_received);

            // Check if packet isnt empty
            if(packet_received.getData() != NULL) {
                prndd("Receive CLIENT");
                sf::Image received_img;
                sf::Vector2i chunk_pos;

                NetworkingManagerUDP::deserializeImage(
                    packet_received, received_img, chunk_pos
                );

                loginf("Received chunk x: ", chunk_pos.x, ".");
                loginf("Received chunk y: ", chunk_pos.y, ".");

                world.getChunkAt(chunk_pos.x, chunk_pos.y).image = received_img;
            }
        }

};