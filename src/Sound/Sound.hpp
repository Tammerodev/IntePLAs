#pragma once
#include <SFML/Audio.hpp>
#include "../common.hpp"

class Sound {
    public: 
        bool load(const std::string& filePath) {
            if(!buffer.loadFromFile(filePath)) {
                prnerr("Could not load audio file. File path is ", filePath);
                return false;
            }

            sound.setBuffer(buffer);

            return true;
        }

        void setLooping(bool loop) {
            sound.setLoop(loop);
        } 

        void setVolume(const float volume) {
            sound.setVolume(volume);
        }

        void play() {
            sound.play();
        }

        void stop() {
            sound.stop();
        }
    private:
        sf::SoundBuffer buffer;
        sf::Sound sound;
};