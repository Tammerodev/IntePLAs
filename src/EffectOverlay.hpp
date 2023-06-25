#pragma once
#include <SFML/Graphics.hpp>

class EffectOverlay {
public:
    int load() {
        int res = 1;
        
        tx.loadFromFile("res/img/Background.png");
        if(!tx.create(1000,1000)) std::cout << "Eroor";
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

    void effect_explosion(const sf::Vector2f&p) {
        shader.setUniform("explosion",sf::Vector2f(p));
        shader_time.restart();
        background.setPosition(p.x - background.getGlobalBounds().width/2, p.y - background.getGlobalBounds().height/2);
    }

    //Todo : Fix shader

private:
    sf::Sprite background;
    sf::Texture tx;
	sf::Shader shader;

	sf::Clock shader_time;
	const char * shader_frag = 
    R"( 
		uniform float time;
        uniform vec2 explosion;
        uniform vec2 worldpos;


        float easeOutQuint(const float x) {
            return 1.0 - pow(1.0 - x, 5.0);
        }


        void main( void ) {

            vec2 position = gl_TexCoord[0].xy  + worldpos / 1000;
            
            vec4 color;
            vec2 exp_pos = explosion / 1000.0;
            
            float dist = distance(position,exp_pos);

            float intens = easeOutQuint(time * 5.0) / 10.0;

            color.a = 1.0 - dist / intens;
            
            color.r = 1.0 - dist / intens;

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