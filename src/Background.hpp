#pragma once
#include <SFML/Graphics.hpp>

class Background {
public:

    const float parallax = 0.2f;
    int load() {
        int res = 1;

		background_tx.loadFromFile("res/world/forest_bg.png");
        background.setTexture(background_tx);
        //background.setOrigin(background_tx.getSize().x/2, background_tx.getSize().y/2);

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

    void update(sf::Vector2f pos) {
        //background.setPosition(pos);
        bg_shader.setUniform("time",shader_time.getElapsedTime().asSeconds());

        sf::Vector2f position = pos;
        float temp = position.x * (1 - parallax);
        float distance = position.x * parallax;

        background.setPosition (0.f + distance, background.getPosition().y);
    }

private:
    sf::Texture background_tx;
    sf::Sprite background;
	sf::Shader bg_shader;

	sf::Clock shader_time;
	const char * shader_frag = 
    R"( 
		
		uniform float time;
		uniform vec2 resolution;
        uniform sampler2D texture;

		void main () {
            vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
            vec4 color;
            if(pixel.a == 0.0) {
                float br = +sin(time / 100.0) / 1.5;
                vec2 pos = gl_FragCoord.xy;
                color.a = 1.0;
                color.b = 0.5;
                color.r = pos.y / 1000.0;

                color.r += br;
                color.g += br;
                color.b += br;
            } else {
                color = pixel;
            }

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