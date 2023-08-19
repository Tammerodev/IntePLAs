#pragma once

#include "UIStateManager.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <thread>
#include <iostream>

#include "State.hpp"
#include "SoundFX.hpp"
#include "Player.hpp"
#include "World.hpp"
#include "math.hpp"
#include "Background.hpp"
#include "EffectOverlay.hpp"
#include "Camera.hpp"
#include "BackgroundMusic.hpp"
#include "PlayerUI.hpp"
#include "Entity/Player/Controls.hpp"

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/CheckBox.hpp>

class GameState : public MainState {
public:
	bool load(const std::string s, tgui::BackendGui& gui);
    void update();
	void input(sf::Event &ev);
	void statexit();

	void draw(sf::RenderWindow& window, tgui::BackendGui&);
private:

	bool GUIfocusedOnObject = false;

	bool slowmo = false;
	sf::Clock clock;

 	sf::Vector2f mousepos;
	sf::Vector2f world_mousepos;
	sf::Vector2f window_mousepos;


	float lastTime = 0;
	float fps = 1;

	Player player;
	Inventory inv;
	MaterialsUI matUI;
	Background bg;
	EffectOverlay effOverlay;
	UIStateManager uiStateManager;

	World world;

	Camera game_camera;
	Camera ui_camera;

	sf::RenderTexture renderTexture;
	sf::Sprite renderSprite;


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
			const float saturation = 1.0;
			vec3 tex_color = pixel.rgb;  

			pixel.rgb = mix(vec3(dot(tex_color.rgb, vec3(0.299, 0.587, 0.114))), tex_color.rgb, saturation);

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