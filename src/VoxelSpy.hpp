#pragma once
#include <list>
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include "Networking/NetworkingManagerUDP.hpp"
#include "VoxelManager.hpp"

#include "Networking/ClientUDP.hpp"

class VoxelSpy {
    public:
        VoxelSpy() {
            client.init();
        }


        void alertOfChunkModification(const sf::Vector2i& chunk_pos, ChunkIndexer &world) {
            chunks_modified.push_back(chunk_pos);

            sf::Packet packet;

            
            packet = NetworkingManagerUDP::serializeImage(
                world.getChunkAt(chunk_pos.x, chunk_pos.y).image,
                chunk_pos
            );
            
            client.send(packet);
        }

        void receiveChunkChanges(ChunkIndexer &world) {
            
        }

    private:
        std::list<sf::Vector2i> chunks_modified;
        Client client;
};