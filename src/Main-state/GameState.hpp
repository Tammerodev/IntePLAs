#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <thread>
#include <iostream>

#include "State.hpp"
#include "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/src/Sound/SoundFX.hpp"
#include "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/src/Entity/Player/Player.hpp"
#include "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/src/VoxelWorld/VoxelManager.hpp"
#include "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/src/VoxelWorld/math.hpp"
#include "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/src/Entity/Player/IObjects/Gun.hpp"

class GameState : public MainState {
public:
	virtual bool load() {
		SFX::load();
		
		background.setSize(sf::Vector2f(100000,100000));
		background.setPosition(0,0);

		player.load();
		view.setSize(800,800);
		view.zoom(0.5);

		vx_manager.load();

		music.openFromFile("/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/res/music/Calm.wav");
		music.play();

		thread1 = std::thread(displayDebugInfo, std::ref(delta_T));
		

		// load only the vertex shader
		(bg_shader.loadFromMemory(shader_vert, sf::Shader::Vertex));
		// load only the fragment shader
		(bg_shader.loadFromMemory(shader_frag, sf::Shader::Fragment));
		// load both shaders
		(bg_shader.loadFromMemory(shader_vert, shader_frag));
		return true;
	}
    virtual void update();
	virtual void input(sf::Event &ev);

	virtual void draw(sf::RenderTarget&window);
private:

	bool slowmo = false;
	static void displayDebugInfo(float &delta_T) {
		while (true)
		{
			std::this_thread::sleep_for(std::chrono::seconds(1));
			std::cout << "Time : " << time(0) << '\n';
			std::cout << "DT :" << delta_T << '\n';
		}
	}

	// Background
	sf::RectangleShape background;
	sf::Shader bg_shader;

	sf::Clock shader_time;
	const char * shader_frag = 
    R"( 
		
		uniform float time;
		uniform vec2 resolution;

		void main () {
			vec2 pos = gl_FragCoord.xy;
			vec4 color;
			color.a = 1.0;

			color.b = 0.5;
			
			color.r = pos.y / 1000.0;
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


	Player player;
	Gun gun = Gun(vx_manager);

	sf::View view;

	sf::Music music;

	VoxelManager vx_manager {};

	float delta_T = 0.f;
	sf::Clock deltaClock;


	std::thread thread1{};
};