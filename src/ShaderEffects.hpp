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
        desaturate_texture.create(width, height);
        return true;
    }

    void update() {
        // Set kernel size
        blur_shader.o_setUniform("kernelSize", 35);

        desaturate_shader.o_setUniform("time", (float)clock.getElapsedTime().asSeconds());
        desaturate_shader.o_setUniform("distortionAmount", ((float)PlayerGlobal::still_radioation) / 100.f);
        desaturate_shader.o_setUniform("isDead", (int)(PlayerGlobal::health <= 0));

        desaturate_shader.o_setUniform("desaturationAmount", (float)GameStatus::brightness);
        desaturate_shader.o_setUniform("temp", (float)PlayerGlobal::bodyTemparature);

    }

    sf::RenderTexture& getTresholdTexture() {
        return treshold_texture;
    }

    void render(sf::RenderTarget& renderTarget, sf::Sprite& originalFrame) {
        // Apply desaturation shader to the original frame
        desaturate_shader.renderTo(originalFrame, desaturate_texture);
        desaturate_texture.display();

        // Draw desaturated frame
        desaturate_sprite.setTexture(desaturate_texture.getTexture());
        renderTarget.draw(desaturate_sprite);

        // Apply threshold shader to the desaturated frame
        treshold_shader.renderTo(desaturate_sprite, treshold_texture);

    }

    void finalRender(sf::RenderTarget& renderTarget) {
        treshold_texture.display();

        // Draw thresholded frame and apply blending
        treshold_sprite.setTexture(treshold_texture.getTexture());
        blur_shader.renderTo(treshold_sprite, treshold_texture_before_blend);

        treshold_texture_before_blend.display();
        treshold_sprite_before_blend.setTexture(treshold_texture_before_blend.getTexture());
        
        renderTarget.draw(treshold_sprite_before_blend, sf::BlendAdd);
    }

    void clearTextures() {
        desaturate_texture.clear(sf::Color::Transparent);
        treshold_texture.clear(sf::Color::Transparent);
        treshold_texture_before_blend.clear(sf::Color::Transparent);
    }

private:
    sf::RenderTexture treshold_texture;
    sf::RenderTexture treshold_texture_before_blend;
    sf::RenderTexture desaturate_texture;

    sf::Sprite treshold_sprite;
    sf::Sprite treshold_sprite_before_blend;
    sf::Sprite desaturate_sprite;

    Shader blur_shader;
    Shader treshold_shader;
    Shader desaturate_shader;

    sf::Clock clock;
};

