#pragma once
#include <SFML/Graphics.hpp>

class EffectOverlay {
public:
    int load() {
        int res = 1;

		background.setSize(sf::Vector2f(100000,100000));
		background.setPosition(0,0);

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
        shader.setUniform("view_poss",view_pos);
    }

    void effect_explosion(const sf::Vector2f&p) {
        shader.setUniform("explosion",p);
    }

    //Todo : Fix shader

private:
    sf::RectangleShape background;
	sf::Shader shader;

	sf::Clock shader_time;
	const char * shader_frag = 
    R"( 
		uniform float time;
        uniform vec2 explosion;
        uniform vec2 view_poss;

		void main () {
			vec2 pos = gl_FragCoord;

			vec4 color;

			color.a = 0.5;
			
            if(distance(pos, vec2(10,10)) < 500.0) {
                color.r = 1.0;
            }

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
            //gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

            // forward the vertex color
            gl_FrontColor = gl_Color;
        }
    )";

};