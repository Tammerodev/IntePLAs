#pragma once
#include <SFML/Graphics.hpp>
#include "Shader.hpp"

#include "Player.hpp"
#include "GameStatus.hpp"

class ShaderEffect {
public:
    bool load(uint16_t width, uint16_t height) {
        blur_shader.load("res/shaders/default_vertex.glsl", "res/shaders/blur_fragment.glsl");
        treshold_shader.load("res/shaders/default_vertex.glsl", "res/shaders/treshold_fragment.glsl");
        desaturate_shader.load("res/shaders/default_vertex.glsl", "res/shaders/desaturate_fragment.glsl");

        treshold_texture.create(width, height);
        treshold_texture_before_blend.create(width, height);
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
        treshold_texture_before_blend.clear(sf::Color::Transparent);

        // Draw original frame to renderTexture using treshold shader
        treshold_shader.renderTo(originalFrame, treshold_texture);


        // Draw the original frame
        desaturate_shader.renderTo(originalFrame, renderTarget);

        // Apply renderTexture to sprite
        treshold_texture.display();
        treshold_sprite.setTexture(treshold_texture.getTexture());

        // Finally render blurred version of pixels exeeding treshold
        blur_shader.renderTo(treshold_sprite, treshold_texture_before_blend);

        treshold_texture_before_blend.display();
        treshold_sprite_before_blend.setTexture(treshold_texture_before_blend.getTexture());

        renderTarget.draw(treshold_sprite_before_blend, sf::BlendAdd);
        
    }
private:
    sf::RenderTexture treshold_texture;
    sf::RenderTexture treshold_texture_before_blend;

    sf::Sprite treshold_sprite;
    sf::Sprite treshold_sprite_before_blend;

    Shader blur_shader;
    Shader treshold_shader;
    Shader desaturate_shader;

    sf::Clock clock;
};

