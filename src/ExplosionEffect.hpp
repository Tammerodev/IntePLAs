#include <SFML/Graphics.hpp>
#include "SoundFX.hpp"
#include "ExplosionInfo.hpp"
#include "Shader.hpp"

class ExplosionEffect {
public:
    void load() {
    
        tx.create(2000,2000);
		background.setPosition(0,0);

            
	    explosion_shader.load("res/shaders/default_vertex.glsl", "res/shaders/explosion_fragment.glsl");

        background.setTexture(tx);
        background.setTextureRect(sf::IntRect(0,0,2000,2000));

        renderTex.create(2000,2000);
        renderTex2.create(2000,2000);
    }

    void update() {

        // TODO FIX This is hacky
        exploding = shader_time.getElapsedTime().asSeconds() < 1.0f;

        if(!exploding) return;

        explosion_shader.setUniform("time",shader_time.getElapsedTime().asSeconds());
        explosion_shader.setUniform("worldpos",background.getPosition());
    }

    void explode(const ExplosionInfo& exInfo) {
        explosion_shader.setUniform("explosion", sf::Vector2f(renderSpr.getGlobalBounds().width/2, renderSpr.getGlobalBounds().height/2));
        explosion_shader.setUniform("str_", exInfo.strength);
        SFX::strong_explosion.play();

        shader_time.restart();
        background.setPosition(0, 0);


        renderSpr.setPosition(0,0);
        renderSpr2.setPosition(exInfo.position.x - renderSpr.getGlobalBounds().width/2, exInfo.position.y - renderSpr.getGlobalBounds().height/2);
    }

    void render(sf::RenderTarget &targ) {
        if(!exploding) return;

        renderTex.clear(sf::Color::Transparent);
        renderTex2.clear(sf::Color::Transparent);
        renderTex.draw(background, &explosion_shader);

        renderSpr.setTexture(renderTex.getTexture());

        renderTex2.draw(renderSpr);

        renderSpr2.setTexture(renderTex2.getTexture());
        
        renderSpr.setPosition(renderSpr2.getPosition());

        targ.draw(renderSpr);

        targ.draw(renderSpr2);

        renderSpr.setPosition(0,0);

    }

    bool getExploding() {
        return exploding;
    }
private:
    bool exploding = false;    
	Shader explosion_shader;

    uint16_t width;
    uint16_t height;


    sf::RenderTexture renderTex;   
    sf::RenderTexture renderTex2;  

    sf::Sprite renderSpr;
    sf::Sprite renderSpr2;

    sf::Sprite background;
    sf::Texture tx;


	sf::Clock shader_time;

};