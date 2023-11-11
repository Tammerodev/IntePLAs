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


class MenuState : public MainState {
public:

 	static void buttonCallBack(const tgui::String path,bool isSettings, tgui::BackendGui& gui) {
		MainState::currentState->statexit();
		gui.removeAllWidgets();

		if(isSettings)
			MainState::currentState = MainState::settingsState;	
		else 
			MainState::currentState = MainState::loadState;

		MainState::currentState->load(path.toStdString(), gui); 
	}

	bool load(const std::string, tgui::BackendGui& gui) {
		const std::string uipath = "res/img/UI/";

		logo = Panel(uipath + "logo.png");
		logo.setPosition(0,0);
		logo.setScale(5,5);

		music.openFromFile("res/music/Recs.wav");
		music.setVolume(100);
		music.play();

		font.loadFromFile("res/Fonts/VT323.ttf");

		try {

			tgui::Theme theme = tgui::Theme("res/themes/nanogui.style");

			int index = 0;
			for (const auto & entry : std::filesystem::directory_iterator(StorageSettings::save_path)) {
				std::string parsed_path = entry.path().string();

				parsed_path.erase(0, StorageSettings::save_path.size());
				prndd(parsed_path);


				auto button = tgui::Button::create(parsed_path);


				// Configure button
				button->setPosition(tgui::Layout2d(50, 200 + index * (16 * 4 + 8)));
				button->setSize(tgui::Layout2d(48 * 4,16 * 4));
				button->setRenderer(theme.getRenderer("Button"));
				button->setWidgetName("Playbutton" + parsed_path);

				button->onPress(buttonCallBack, button->getText(), false, std::ref(gui));

				gui.add(button);

				

				++index;

			}

			auto settingsButton = tgui::Button::create("settings");
			settingsButton->setPosition(100,100);
			settingsButton->onPress(buttonCallBack, settingsButton->getText(), true, std::ref(gui));

			gui.add(settingsButton);
		} catch(std::exception& ex) {
			prnerr("TGUI failed with : ", ex.what());
		}

		return true;
	}
    void update() {
		for(auto & bg : background) {
			bg.move(0, -1.f);
		}
	}
	void input(sf::Event &e) {

	}
	void draw(sf::RenderWindow& window, tgui::BackendGui& gui) {
		window.clear(sf::Color(20, 22, 33));

		logo.applyTexture();

		for(auto & bg : background) {
			window.draw(bg);
		}

		window.draw(logo);
	}
	void statexit() {
		background.clear();
		
		music.stop();
	}
private:

	std::vector<sf::Texture> tx;
	std::vector<sf::Sprite> background;


	sf::Font font;

	Panel logo;
	sf::Music music;
};