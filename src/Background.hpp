#pragma once
#include <SFML/Graphics.hpp>

class Background {
public:
    int load() {
        int res = 1;

		background.setSize(sf::Vector2f(100000,100000));
		background.setPosition(0,0);

		// load only the vertex shader
		if(!bg_shader.loadFromMemory(shader_vert, sf::Shader::Vertex)) res = 0;
		// load only the fragment shader
		if(!bg_shader.loadFromMemory(shader_frag, sf::Shader::Fragment)) res = 0;
		// load both shaders
		if(!bg_shader.loadFromMemory(shader_vert, shader_frag)) res = 0;
		return res;
    }

    void render(sf::RenderTarget &targ) {
        targ.draw(background, &bg_shader);
    }

    void update() {
        bg_shader.setUniform("time",shader_time.getElapsedTime().asSeconds());
    }

private:
    sf::RectangleShape background;
	sf::Shader bg_shader;

	sf::Clock shader_time;
	const char * shader_frag = 
    R"( 
		
		uniform float time;
		uniform vec2 resolution;

		void main () {
            float br = +sin(time) / 20.0;

			vec2 pos = gl_FragCoord.xy;

			vec4 color;

			color.a = 1.0;
			color.b = 0.5;
			color.r = pos.y / 1000.0;

            color.r += br;
            color.g += br;
            color.b += br;

			gl_FragColor =  color;
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