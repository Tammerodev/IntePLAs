#pragma once
#include "State.hpp"
#include "Button.hpp"
#include "Panel.hpp"
#include <filesystem>
#include <SFML/Audio.hpp>

#include <TGUI/TGUI.hpp>
#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/Core.hpp>
#include <TGUI/Widgets/Button.hpp>
#include <TGUI/Widgets/CheckBox.hpp>

#include "Settings.hpp"
#include "Palettes/PaletteUI.hpp"

#include "WorldSelectionState.hpp"
#include <iostream>
#include "common.hpp"
#include "MenuBackground.hpp"

class MenuState : public MainState {
public:

 	static void buttonCallBack(const tgui::String path, int destination, tgui::BackendGui& gui) {
		MainState::currentState->statexit();
		gui.removeAllWidgets();

		if(destination == 1)
			MainState::currentState = MainState::settingsState;	
		else if(destination == 3)
			MainState::currentState = MainState::worldSelectionState;

		MainState::currentState->load(path.toStdString(), gui); 
	}

	bool load(const std::string, tgui::BackendGui& gui) {
		const std::string uipath = "res/img/UI/";
		SettingsLoader::loadSettings();

		backend_gui = &gui;
		MenuMusic::load();
		MenuMusic::play();

		font.loadFromFile("res/Fonts/VT323.ttf");

		MenuBackground::load();

		try {

			gui.loadWidgetsFromFile("res/ui/start_menu.txt");
			
		} catch(std::exception& ex) {
			prnerr("TGUI failed with : ", ex.what());
		}
		return true;
	}
    void update() {
		for(auto & bg : background) {
			bg.move(0, -1.f);
		}

		MenuBackground::update();
	}
	void input(sf::Event &e) {
		if(backend_gui == nullptr) return;

		if(e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Button::Left) {

			auto PlayButton = backend_gui->get("Play");
			if(PlayButton->isMouseOnWidget(sf::Vector2f(e.mouseButton.x, e.mouseButton.y))) {
				buttonCallBack("Create new world", 3, *backend_gui);
			}

			auto SettingsButton = backend_gui->get("Settings");
			if(SettingsButton != nullptr && SettingsButton->isMouseOnWidget(sf::Vector2f(e.mouseButton.x, e.mouseButton.y))) {
				buttonCallBack("", 1, *backend_gui);
			}

			auto HostButton = backend_gui->get("Host");
			if(HostButton != nullptr && HostButton->isMouseOnWidget(sf::Vector2f(e.mouseButton.x, e.mouseButton.y))) {
				buttonCallBack("", 2, *backend_gui);
			}
		}
	}
	void draw(sf::RenderWindow& window, tgui::BackendGui& gui) {
		window.clear(Palette::PaletteUI::Black);

		MenuBackground::render(window);

		for(auto & bg : background) {
			window.draw(bg);
		}	
		
   		gui.draw();

	}
	void statexit() {
		background.clear();
	}
private:

	tgui::BackendGui *backend_gui = nullptr;

	std::vector<sf::Texture> tx;
	std::vector<sf::Sprite> background;

	sf::Font font;
};