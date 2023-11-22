#pragma once
#include <SFML/Graphics.hpp>
#include "Shader.hpp"

class ShaderEffect {
public:
    bool load(uint16_t width, uint16_t height) {
        blur_shader.load("res/shaders/default_vertex.glsl", "res/shaders/blur_fragment.glsl");
        treshold_shader.load("res/shaders/default_vertex.glsl", "res/shaders/treshold_fragment.glsl");
        desaturate_shader.load("res/shaders/default_vertex.glsl", "res/shaders/desaturate_fragment.glsl");

        treshold_texture.create(width, height);
        return true;
    }

    void update() {
        // Set kernel size
        blur_shader.o_setUniform("kernelSize", 35 + (int)GameStatus::brightness * 10);

        desaturate_shader.o_setUniform("time", (float)clock.getElapsedTime().asSeconds());
        desaturate_shader.o_setUniform("distortionAmount", ((float)PlayerGlobal::still_radioation) / 10.f);
    }

    void render(sf::RenderTarget& renderTarget, sf::Sprite& originalFrame) {
        // Clear renderTexture
        treshold_texture.clear(sf::Color::Transparent);

        // Draw original frame to renderTexture using treshold shader
        treshold_shader.renderTo(originalFrame, treshold_texture);


        // Draw the original frame
        desaturate_shader.renderTo(originalFrame, renderTarget);

        // Apply renderTexture to sprite
        treshold_texture.display();
        treshold_sprite.setTexture(treshold_texture.getTexture());

        // Finally render blurred version of pixels exeeding treshold
        blur_shader.renderTo(treshold_sprite, renderTarget);  
    }
private:
    sf::RenderTexture treshold_texture;
    sf::Sprite treshold_sprite;

    Shader blur_shader;
    Shader treshold_shader;
    Shader desaturate_shader;

    sf::Clock clock;
};

