#pragma once
#include <SFML/Graphics.hpp>
#include "SoundFX.hpp"
#include "ExplosionInfo.hpp"

class EffectOverlay {
public:
    int load() {
        int res = 1;
    
        if(!tx.create(2000,2000)) res = 0;
		background.setPosition(0,0);

               
        background.setTexture(tx);
        background.setTextureRect(sf::IntRect(0,0,2000,2000));

		if(!explosion_shader.loadFromFile("res/shaders/default_vertex.glsl", "res/shaders/explosion_fragment.glsl")) res = 0;

        if(!blur_shader.loadFromFile("res/shaders/default_vertex.glsl", "res/shaders/blur_fragment.glsl")) res = 0;

        if(!treshold_shader.loadFromFile("res/shaders/default_vertex.glsl", "res/shaders/treshold_fragment.glsl")) res = 0;


        renderTex.create(2000,2000);
        renderTex2.create(2000,2000);

		return res;
    }

    void render(sf::RenderTarget &targ) {
        renderTex.clear(sf::Color::Transparent);
        renderTex2.clear(sf::Color::Transparent);
        renderTex.draw(background, &explosion_shader);

        renderSpr.setTexture(renderTex.getTexture());

        renderTex2.draw(renderSpr, &treshold_shader);

        renderSpr2.setTexture(renderTex2.getTexture());
        
        renderSpr.setPosition(renderSpr2.getPosition());

        targ.draw(renderSpr);

        targ.draw(renderSpr2, &blur_shader);

        renderSpr.setPosition(0,0);

    }

    void update(const sf::Vector2f &view_pos) {
        explosion_shader.setUniform("time",shader_time.getElapsedTime().asSeconds());
        explosion_shader.setUniform("worldpos",background.getPosition());

    }

    void effect_explosion(const ExplosionInfo& exInfo) {
        explosion_shader.setUniform("explosion", sf::Vector2f(renderSpr.getGlobalBounds().width/2, renderSpr.getGlobalBounds().height/2));
        explosion_shader.setUniform("str_", exInfo.strength);
        SFX::strong_explosion.play();

        shader_time.restart();
        background.setPosition(0, 0);


        renderSpr.setPosition(0,0);
        renderSpr2.setPosition(exInfo.position.x - renderSpr.getGlobalBounds().width/2, exInfo.position.y - renderSpr.getGlobalBounds().height/2);
    }

    //Todo : Fix shader

private:

    uint16_t width;
    uint16_t height;


    sf::RenderTexture renderTex;   
    sf::RenderTexture renderTex2;  

    sf::Sprite renderSpr;
    sf::Sprite renderSpr2;

    sf::Sprite background;
    sf::Texture tx;
	sf::Shader explosion_shader;
    sf::Shader blur_shader;
    sf::Shader treshold_shader;


	sf::Clock shader_time;

};