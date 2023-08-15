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


class MenuState : public MainState {
public:

 	static void buttonCallBack(const tgui::String path, tgui::BackendGui& gui) {
		MainState::currentState = MainState::gameState;
		MainState::currentState->load(path.toStdString(), gui); 

		gui.removeAllWidgets();
	}

	bool load(const std::string, tgui::BackendGui& gui) {
		const std::string uipath = "res/img/UI/";
		logo = Panel(uipath + "logo.png");
		logo.setPosition(0,0);
		logo.setScale(5,5);

		music.openFromFile("res/music/Bream.wav");
		music.setVolume(0);
		music.play();

		playbtn_tx.loadFromFile(uipath + "playbtn.png");

		font.loadFromFile("res/Fonts/VT323.ttf");

		try {

			tgui::Theme theme = tgui::Theme("res/themes/nanogui.style");

			int index = 0;
			for (const auto & entry : std::filesystem::directory_iterator(path)) {
				std::string parsed_path = entry.path().string();
				// Remove the res/saves/ substring
				parsed_path.erase(0, 10);
				// Remove the .png file extension
				parsed_path.erase(parsed_path.size() - 4, parsed_path.size());

				auto button = tgui::Button::create(parsed_path);

				// Configure button
				button->setPosition(tgui::Layout2d(50, 200 + index * (16 * 4 + 8)));
				button->setSize(tgui::Layout2d(48 * 4,16 * 4));
				button->setRenderer(theme.getRenderer("Button"));

				button->onPress(buttonCallBack, button->getText(), std::ref(gui));

				gui.add(button);

				

				++index;

			}
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
		if(e.type == sf::Event::Closed) exit(0);
	}
	void draw(sf::RenderTarget& window, tgui::BackendGui& gui) {

		window.clear(sf::Color(20, 22, 33));

		logo.applyTexture();

		for(auto & bg : background) {
			window.draw(bg);
		}
		window.draw(logo);

		for(auto &i : clickables) {
			window.draw(*i);
			i->drawText(window);

			const sf::Vector2f pos = sf::Vector2f(window.mapCoordsToPixel(sf::Vector2f(sf::Mouse::getPosition())));

			if(i->getState(pos) == Button::ButtonState::Click) { 
				statexit();
				MainState::currentState = MainState::gameState;
				MainState::currentState->load(i->getText(), gui); 
			}
			if(i->getState(pos) == Button::ButtonState::Hover) { 
				i->setTextureRect(sf::IntRect(0,24,64,24));
			} else {
				i->setTextureRect(sf::IntRect(0,0,64,24));
			}
		}
	}
	void statexit() {
		music.stop();
	}
private:

	std::vector<sf::Texture> tx;
	std::vector<sf::Sprite> background;
	std::vector<Button*> clickables;
	Button new_world_button;

	sf::Font font;

	std::string path = "res/saves/";
	Panel logo;
	sf::Music music;

	sf::Texture playbtn_tx;
};