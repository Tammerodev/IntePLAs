#pragma once
#include <SFML/Graphics.hpp>
#include "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/src/ExplosionInfo.hpp"

class EffectOverlay {
public:
    int load() {
        int res = 1;
    
        if(!tx.create(1000,1000)) res = 0;
		background.setPosition(0,0);

               
        background.setTexture(tx);
        background.setTextureRect(sf::IntRect(0,0,1000,1000));

		// load only the vertex shader
		if(!shader.loadFromMemory(shader_vert, sf::Shader::Vertex)) res = 0;
		// load only the fragment shader
		if(!shader.loadFromMemory(shader_frag, sf::Shader::Fragment)) res = 0;
		// load both shaders
		if(!shader.loadFromMemory(shader_vert, shader_frag)) res = 0;
		return res;
    }

    void render(sf::RenderTarget &targ) {
        targ.draw(background, &shader);
    }

    void update(const sf::Vector2f &view_pos) {
        shader.setUniform("time",shader_time.getElapsedTime().asSeconds());
        shader.setUniform("worldpos",background.getPosition());

    }

    void effect_explosion(const ExplosionInfo& exInfo) {
        shader.setUniform("explosion", sf::Vector2f(exInfo.position));
        shader.setUniform("str_", exInfo.strength);
        SFX::strong_explosion.play();

        shader_time.restart();
        background.setPosition(exInfo.position.x - background.getGlobalBounds().width/2, exInfo.position.y - background.getGlobalBounds().height/2);
    }

    //Todo : Fix shader

private:
    sf::Sprite background;
    sf::Texture tx;
	sf::Shader shader;

	sf::Clock shader_time;
	const char * shader_frag = 
    R"( 
        uniform sampler2D texture;
		uniform float time;
        uniform vec2 explosion;
        uniform vec2 worldpos;
        uniform float str_;

        float easeOutQuint(const float x) {
            return 1.0 - pow(1.0 - x, 5.0);
        }


        float random(float seed) {
            return fract(sin(seed) * 43758.5453);
        }

        void main( void ) {
            vec2 position = gl_TexCoord[0].xy + worldpos / 1000;

            const float pixelate = 200.0;
            
            vec2 exp_pos = explosion / 1000.0;

            position.x = floor(position.x * pixelate);
            position.x /= pixelate;

            position.y = floor(position.y * pixelate);
            position.y /= pixelate;
            
            float dist = floor(
                distance(position,exp_pos) * pixelate
                );
            dist /= pixelate;

            float intens = easeOutQuint(time * 5.0) * str_ / 500.0;
	
            vec4 color;
            
            color.r = 1.0 - dist / intens;
            color.b = intens - 1.0 * dist;

            color.r = floor(color.r*pixelate);
            color.r /= pixelate;
            color.g = floor(color.g*pixelate);
            color.g /= pixelate;
            color.b = floor(color.b*pixelate);
            color.b /= pixelate;

            color.a = color.r;

            gl_FragColor = color;
        }
			
    )";

    const char * shader_vert = 
    R"( 
        void main()
        {
            // transform the vertex position
            gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

            // transform the texture coordinates
            gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

            // forward the vertex color
            gl_FrontColor = gl_Color;
        }
    )";

};