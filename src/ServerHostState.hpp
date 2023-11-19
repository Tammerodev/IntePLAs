#pragma once
#include "State.hpp"
#include "VoxelManager.hpp"
#include "Camera.hpp"

#include "Networking/NetworkingManagerUDP.hpp"
#include "Networking/ServerUDP.hpp"

class ServerHostState : public MainState {
    public:

        bool load(const std::string path, tgui::BackendGui&) {
            const uint16_t window_height = sf::VideoMode::getDesktopMode().height;
            const uint16_t window_width = sf::VideoMode::getDesktopMode().width;

            view.setSize(sf::Vector2u(window_width, window_height));

            Session::session = Session::Host;

            voxelManager.load(path);

            if(path == "Create new world") {
                voxelManager.generate();
                voxelManager.generateVegetation();
            }

            voxelManager.initVoxelMap();

            server.init();
        

            
            return true;
        }
    
        void update() {
            float speed = 1.5;

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
                speed = 7.5;

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                view.move(0, -speed);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
                view.move(0, speed);

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
                view.move(-speed, 0);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
                view.move(speed, 0);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
                view.zoom(0.99);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
                view.zoom(1.01);
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
                voxelManager.updateAll();

                //voxelManager.initVoxelMap();
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I)) {
                /*for(int64_t x = 0; x < chunks_x; x++) {
                    for(int64_t y = 0; y < chunks_y; y++) {
                        sf::Packet packet = NetworkingManagerUDP::serializeImage(voxelManager.getChunkIndexer().getChunkAt(x, y).image, sf::Vector2i(x, y));
                        server.send(packet);

                        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
                    }
                }*/
                prndd("SENDING!!!");
                sf::Packet packet;
                packet << "Hei!";

                server.send(packet);
            }


            sf::Packet packet_received;
            server.receive(packet_received);

            // Check if packet isnt empty
            if(packet_received.getData() != NULL) {
                prndd("Receive");
                sf::Image received_img;
                sf::Vector2i chunk_pos;

                NetworkingManagerUDP::deserializeImage(
                    packet_received, received_img, chunk_pos
                );

                loginf("Received chunk x: ", chunk_pos.x, ".");
                loginf("Received chunk y: ", chunk_pos.y, ".");

                voxelManager.getChunkIndexer().getChunkAt(chunk_pos.x, chunk_pos.y).image = received_img;
            }

        }

        void input(sf::Event &e) {

        }

        void draw(sf::RenderWindow& window, tgui::BackendGui&) {
            window.clear(sf::Color(6, 12, 24));

            view.setViewTo(window);
            voxelManager.render(window, view.getCenterPosition());
        }

        void statexit() {

        }

    private:
        Server server;

        VoxelManager voxelManager;

        Camera view;
};