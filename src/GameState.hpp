#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <thread>
#include <iostream>

#include "State.hpp"
#include "SoundFX.hpp"
#include "Entity/Player/Player.hpp"
#include "VoxelManager.hpp"
#include "math.hpp"
#include "Background.hpp"
#include "EffectOverlay.hpp"
#include "Camera.hpp"
#include "BackgroundMusic.hpp"
#include "Inventory.hpp"

class GameState : public MainState {
public:
	bool load(const std::string s) {

		if(SFX::rocket_launcher_fire_buffer.getDuration() != sf::Time::Zero) return true;

		// TODO : adjustable rendertx size
		const uint16_t window_height = 1200;
		const uint16_t window_width = 1200;
        renderTexture.create(window_width, window_height);
		
		game_camera.setSize(sf::Vector2u(window_width, window_height));
		ui_camera.setSize(sf::Vector2u(window_width, window_height));
		
		inv.load(vx_manager);
		game_camera.setZoom(0.5f);
		ui_camera.setZoom(1.0f);

		game_camera.setLeapSpeed(1.f);
		game_camera.setCameraMode(CameraMode::Leap);


		if(!vx_manager.load(s))
			perror("VoxelManager failed to load");
		if(!bg.load()) 
			perror("Background failed to load");
		if(!effOverlay.load()) 
			perror("Effect Overlay failed to load");
		if(!player.load()) 
			perror("Player failed to load");
		if(!SFX::load())
			perror("Failed to load sound effect");
		if(!BGMusic::load())
			perror("Failed to load background music");

		// load only the vertex shader
		if(!shader.loadFromMemory(shader_vert, sf::Shader::Vertex));
		// load only the fragment shader
		if(!shader.loadFromMemory(shader_frag, sf::Shader::Fragment));
		// load both shaders
		if(!shader.loadFromMemory(shader_vert, shader_frag));

		return true;
	}
    void update();
	void input(sf::Event &ev);
	void statexit();

	void draw(sf::RenderTarget&window);
private:
	bool slowmo = false;
	sf::Clock clock;

	float lastTime = 0;
	float fps = 1;

	Player player;
	Inventory inv;
	Background bg;
	EffectOverlay effOverlay;

	Camera game_camera;
	Camera ui_camera;

	sf::RenderTexture renderTexture;
	sf::Sprite renderSprite;

	VoxelManager vx_manager {};

	float delta_T = 0.f;
	sf::Clock deltaClock;

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