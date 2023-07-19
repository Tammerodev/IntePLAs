#pragma once
#include <SFML/Graphics.hpp>
#include "ParallaxLayer.hpp"
#include "common.hpp"

class Background {
public:
    int load() {
        int res = 1;
		// load only the vertex shader
		if(!bg_shader.loadFromMemory(shader_vert, sf::Shader::Vertex)) res = 0;
		// load only the fragment shader
		if(!bg_shader.loadFromMemory(shader_frag, sf::Shader::Fragment)) res = 0;
		// load both shaders
		if(!bg_shader.loadFromMemory(shader_vert, shader_frag)) res = 0;

        parallax.push_back(ParallaxLayer("res/world/forest_bg_layer2.png", 0.05f));
        parallax.push_back(ParallaxLayer("res/world/forest_bg_layer1.png", 0.1f));

		return res;
    }

    void render(sf::RenderTarget &targ) {
        for(auto &layer : parallax) {
            layer.render(targ);
        }
    }

    void update(sf::Vector2f pos) {
        for(auto &layer : parallax) {
            layer.update(pos);
        }
        bg_shader.setUniform("time",shader_time.getElapsedTime().asSeconds());
    }

private:
    std::vector<ParallaxLayer> parallax;
	sf::Shader bg_shader;

	sf::Clock shader_time;
	const char * shader_frag = 
    R"( 
		
		uniform float time;
		uniform vec2 resolution;
        uniform sampler2D texture;

		void main () {
            vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

			gl_FragColor = pixel;
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