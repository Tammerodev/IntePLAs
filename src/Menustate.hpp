#pragma once
#include "State.hpp"
#include "Button.hpp"
#include "Panel.hpp"
#include <filesystem>
#include <SFML/Audio.hpp>

class MenuState : public MainState {
public:
	bool load(const std::string) {
		logo = Panel("res/img/title/logo.png");
		logo.setPosition(0,0);
		logo.setScale(5,5);

		music.openFromFile("res/music/Bream.wav");
		music.play();

		playbtn_tx.loadFromFile("res/img/title/playbtn.png");

		font.loadFromFile("res/Fonts/VT323.ttf");
		int index = 0;
		for (const auto & entry : std::filesystem::directory_iterator(path)) {
			Button* b = new Button(playbtn_tx, sf::Vector2f(150, 200 + index * 50), sf::Vector2f(2,2), sf::IntRect(0,0,64,24), font);
			b->setString(entry.path());
			sf::Texture tex;
			tex.loadFromFile(entry.path());
			tx.push_back(tex);
			b->move(-150,0);
        	clickables.push_back(b);
			++index;
		}

		index = 0;
		for(auto &tex : tx) {
			sf::Sprite bg;

			bg.setTexture(tex);
			bg.setPosition(0, index * tex.getSize().y);
			
			background.push_back(bg);
			index++;
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
	void draw(sf::RenderTarget& window) {
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
				MainState::currentState->load(i->getText()); 
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