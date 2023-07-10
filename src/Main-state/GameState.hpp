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
#include "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/src/Background.hpp"
#include "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/src/EffectOverlay.hpp"
#include "/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/src/Camera.hpp"

class GameState : public MainState {
public:
	virtual bool load() {
		// TODO : adjustable rendertx size
		const uint16_t window_height = 1200;
		const uint16_t window_width = 1200;
        renderTexture.create(window_width, window_height);
		
		game_camera.setSize(sf::Vector2u(window_width, window_height));
		ui_camera.setSize(sf::Vector2u(window_width, window_height));

		game_camera.setZoom(0.5f);
		ui_camera.setZoom(1.0f);

		game_camera.setLeapSpeed(1.f);
		game_camera.setCameraMode(CameraMode::Leap);


		if(!vx_manager.load())
			perror("VoxelManager failed to load");
		if(!bg.load()) 
			perror("Background failed to load");
		if(!effOverlay.load()) 
			perror("Effect Overlay failed to load");
		if(!player.load()) 
			perror("Player failed to load");
		if(!SFX::load())
			perror("Failed to load sound effect");
			
		music.openFromFile("/media/lauri/acc1d3fc-a54d-465a-b6f6-116e7faa91c3/IntePLAs/res/music/Calm.wav");
		music.play();

		thread1 = std::thread(displayDebugInfo, std::ref(delta_T));

		// load only the vertex shader
		if(!shader.loadFromMemory(shader_vert, sf::Shader::Vertex));
		// load only the fragment shader
		if(!shader.loadFromMemory(shader_frag, sf::Shader::Fragment));
		// load both shaders
		if(!shader.loadFromMemory(shader_vert, shader_frag));

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

	Player player;
	Gun gun = Gun(vx_manager);
	Background bg;
	EffectOverlay effOverlay;

	Camera game_camera;
	Camera ui_camera;

	sf::RenderTexture renderTexture;
	sf::Sprite renderSprite;

	sf::Music music;

	VoxelManager vx_manager {};

	float delta_T = 0.f;
	sf::Clock deltaClock;


	std::thread thread1{};

	sf::Shader shader;

	sf::Clock shader_time;
	const char * shader_frag = 
    R"( 
		uniform sampler2D texture;

		uniform float time;

		void main()
		{
			// lookup the pixel in the texture
			vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

			// multiply it by the color
			gl_FragColor = gl_Color * pixel;
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