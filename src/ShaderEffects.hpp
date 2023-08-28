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
        blur_shader.setUniform("kernelSize", 23 + (int)GameStatus::brightness * 10);

        desaturate_shader.setUniform("targetColor", sf::Vector3f(0.04, 0.02, 0.1));
        desaturate_shader.setUniform("amount", GameStatus::brightness);

    }

    void render(sf::RenderTarget& renderTarget, const sf::Sprite& originalFrame) {
        // Clear renderTexture
        treshold_texture.clear(sf::Color::Transparent);

        // Draw original frame to renderTexture using treshold shader
        treshold_texture.draw(originalFrame, &treshold_shader);


        // Draw the original frame
        renderTarget.draw(originalFrame, &desaturate_shader);

        // Apply renderTexture to sprite
        treshold_texture.display();
        treshold_sprite.setTexture(treshold_texture.getTexture());

        // Finally render blurred version of pixels exeeding treshold
        renderTarget.draw(treshold_sprite, &blur_shader);
        
    }
private:
    sf::RenderTexture treshold_texture;
    sf::Sprite treshold_sprite;

    Shader blur_shader;
    Shader treshold_shader;
    Shader desaturate_shader;
};

