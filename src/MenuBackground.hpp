#pragma once
#include <string>
#include "common.hpp"
#include <SFML/Graphics.hpp>
#include "Settings.hpp"
#include <filesystem>
#include "Shader.hpp"

namespace MenuMusic {
    inline sf::Music music;

    inline bool load() {
        music.setVolume(SoundSettings::music_volume);
        return music.openFromFile("res/music/Recs.wav");
    }

    inline void play() {
        music.play();
    }

    inline void stop() {
        music.stop();
    }
}

namespace MenuBackground {
    inline bool changed = false;

    inline sf::Texture texture;
    inline sf::Sprite sprite;

    inline sf::Texture new_texture;
    inline sf::Sprite new_sprite;

    inline sf::Clock clock;
    inline Shader shader;
    
    inline void load() {
        texture.loadFromFile(StorageSettings::screenshot_path + "start.png");

        sprite.setTexture(texture);
        sprite.setPosition(0, 0);
        new_sprite.setPosition(0, 0);

        shader.load("res/shaders/default_vertex.glsl", "res/shaders/imageBlur.glsl");
    }

    inline void render(sf::RenderTarget& target) {
        target.draw(sprite);
        target.draw(new_sprite);

    }

    inline void update() {
        int time = clock.getElapsedTime().asSeconds();
        sprite.setTexture(texture);

        sprite.setScale(1.1, 1.1);
        new_sprite.setScale(1.1, 1.1);


        if(time > 15) {
            clock.restart();

            // Load random texture from folder
            std::string folderPath = StorageSettings::screenshot_path;
            std::vector<std::string> textureNames;

            // Iterate through the files in the folder
            for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
                if (entry.is_regular_file()) {
                    textureNames.push_back(entry.path().filename().string());
                }
            }

            // Shuffle the vector to get a random order
            std::random_shuffle(textureNames.begin(), textureNames.end());

            // Load the first texture (randomly selected) from the shuffled vector
            if (new_texture.loadFromFile(folderPath + textureNames[0])) {
                // Texture loaded successfully, use it in your game or application
                std::cout << "Loaded texture: " << textureNames[0] << std::endl;
            } else {
                std::cerr << "Failed to load texture: " << textureNames[0] << std::endl;
            }

            new_sprite.setTexture(new_texture);
            new_sprite.setColor(sf::Color(255, 255, 255, 0));

            changed = true;
        }

        if(changed) {
            sf::Color color = sf::Color(255, 255, 255, 0);
            color.a = new_sprite.getColor().a;

            if(color.a < 254) {
                color.a++;
            } else {
                sprite.setPosition(0, 0);
                texture = (new_texture);
                color.a = 0;
                changed = false;
            }

            new_sprite.setColor(color);
        } else {
            new_sprite.setColor(sf::Color(255, 255, 255, 0));
            sprite.move(-0.05, 0);
        }
    }
};
