#pragma once
#include <SFML/Graphics.hpp>
#include "Palettes/PaletteUI.hpp"
#include <iostream>
#include "debug_globals.hpp"

namespace LoadingScreen {
    inline static sf::Text text_prompt;
    inline static sf::Text text_state;


    inline static sf::Font font;
    inline static sf::Texture loading_texture;
    inline static sf::Sprite loading_sprite;

    inline int animation_phase = 0;
    inline int animation_timer = 0;
    inline int animation_time = 3;

    inline static void loadText(const sf::Vector2f windowSize) {
        sf::Vector2f text_position(windowSize);

        font.loadFromFile("res/Fonts/VT323.ttf");
        text_prompt.setFont(font);
        text_prompt.setString("Loading the game... This is actually running on a seperate thread!"
                              "\n Remember, window.setActive(false)"
                              " when running on multiple threads!");
        text_position.x /= 2;
        text_position.y -= text_prompt.getGlobalBounds().height + 64;
        text_position.x -= text_prompt.getGlobalBounds().width / 2;

        text_prompt.setPosition(text_position);

        text_state.setFont(font);
        text_state.setPosition(text_position - sf::Vector2f(0, 50));
    }

    inline static void loadSprites(const sf::Vector2f windowSize) {
        if(!loading_texture.loadFromFile("res/img/UI/loading.png")) {
            std::cerr << "Could not load animation!\n";
        }
        loading_sprite.setTexture(loading_texture);
    }

    inline static void updateSprites(const sf::Vector2f windowSize) {
        
        animation_timer++;

        if(animation_timer > animation_time) {
            animation_timer = 0;
            animation_phase++;
        }

        loading_sprite.setScale(1.0f + animation_timer / animation_time, 1.0f + animation_timer / animation_time);

        if(animation_phase > 13) {
            animation_phase = 0;
        }
        if(animation_phase == 6) {
            loading_sprite.setColor(sf::Color(rand() % 255, rand() % 255, rand() % 255));
        }

        sf::IntRect spriteArea = sf::IntRect(animation_phase * 32, 0, 32, 32);
        loading_sprite.setTextureRect(spriteArea);
        loading_sprite.setTexture(loading_texture);


        loading_sprite.setScale(4.5, 4.5);

        sf::Vector2f position = windowSize;
        position.x /= 2;
        position.y -= 400;
        position.x -= loading_sprite.getGlobalBounds().width / 2;

        loading_sprite.setPosition(position);
    }

    inline static void load_screen(sf::RenderWindow &window, const Game &game, sf::Vector2u size) {        
        // Make window active
        window.setActive(true);

        std::cout << "Size X " << size.x << '\n';
        std::cout << "Size Y " << size.y << '\n';



        loadText(sf::Vector2f(size));
        loadSprites(sf::Vector2f(size));

        while(game.getLoaded() == false) {
            updateSprites(sf::Vector2f(size));

            text_state.setString(load_state::getNameOfLoadState(load_state::loadState));


            window.clear(Palette::PaletteUI::Black);

            window.draw(text_prompt);
            window.draw(text_state);

            window.draw(loading_sprite);

            window.display();
        }

        // Disable window on this thread
        window.setActive(false);

    }
}