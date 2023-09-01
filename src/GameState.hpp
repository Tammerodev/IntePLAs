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
#include "Controls.hpp"
#include "Cursor.hpp"
#include "GameStatus.hpp"

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/CheckBox.hpp>

#include "JsonManager.hpp"
#include "ShaderEffects.hpp"

class GameState : public MainState {
public:
	bool load(const std::string s, tgui::BackendGui& gui);
    void update();
	void input(sf::Event &ev);
	void statexit();

	uint64_t timer = 0.f;

	const sf::Event event_sig() {
		
		sf::Event tEvent;

		if(Controls::useUI()) {

			timer += 1;
			sf::Event::MouseButtonEvent press;
			press.button = sf::Mouse::Button::Left;
			press.x = Controls::windowCursorPos.x;
			press.y = Controls::windowCursorPos.y;

			if(timer < 5)
				tEvent.type = sf::Event::MouseButtonPressed;
			else {
				tEvent.type = sf::Event::MouseButtonReleased;
				timer = 0;
			}
			tEvent.mouseButton = press;
		} 
		else {
			tEvent.type = sf::Event::SensorChanged;
		}


		return tEvent;
	}

	void draw(sf::RenderWindow& window, tgui::BackendGui&);
private:

	

	bool GUIfocusedOnObject = false;

	bool slowmo = false;
	sf::Clock clock;


	float lastTime = 0;
	float fps = 1;

	Player player;
	Inventory inv;
	MaterialsUI matUI;
	Background bg;
	EffectOverlay effOverlay;
	UIStateManager uiStateManager;

	Cursor cursor;

	World world;

	Camera game_camera;
	Camera ui_camera;

	sf::RenderTexture renderTexture;
	sf::Sprite renderSprite;

	ShaderEffect shaderEffect;


	float delta_T = 0.f;
	sf::Clock deltaClock;

	sf::Clock shader_time;

};